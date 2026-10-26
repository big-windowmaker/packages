#!/usr/bin/env bash
# Compila um diretório de pacote (pkgbase) dentro de um contêiner Manjaro
# descartável e roda o namcap no PKGBUILD e nos pacotes gerados.
#
# Uso (como root, dentro do contêiner):
#   ci/build-package.sh <diretório-do-pacote> [diretório-de-saída]
#
# O mesmo script é usado pelo GitHub Actions e pelo build local com podman
# (ver README.md). Ele não usa nenhum segredo e não publica nada.
set -euo pipefail

pkgdir_arg=${1:?informe o diretório do pacote}
out_dir=${2:-/out}
repo_root=$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)

# Aceita apenas nomes simples de diretório (sem barras ou "..").
if [[ ! ${pkgdir_arg} =~ ^[a-z0-9][a-z0-9._+-]*$ ]] || [[ ! -f ${repo_root}/${pkgdir_arg}/PKGBUILD ]]; then
  echo "diretório de pacote inválido: ${pkgdir_arg}" >&2
  exit 2
fi

if [[ ${EUID} -ne 0 ]]; then
  echo "este script deve rodar como root dentro do contêiner de build" >&2
  exit 2
fi

# Toolchain mínima. O makepkg roda como usuário sem privilégios; esse usuário
# só pode chamar o pacman via sudo (para instalar as dependências do build).
if [[ ! -f /etc/pacman.d/gnupg/trustdb.gpg ]]; then
  pacman-key --init
  pacman-key --populate archlinux manjaro
fi
pacman -Syu --noconfirm --needed base-devel git namcap sudo
id builder >/dev/null 2>&1 || useradd -m builder
echo 'builder ALL=(root) NOPASSWD: /usr/bin/pacman' > /etc/sudoers.d/builder
chmod 0440 /etc/sudoers.d/builder

work=$(mktemp -d /tmp/build.XXXXXX)
cp -r "${repo_root}/${pkgdir_arg}" "${work}/"
chown -R builder: "${work}"
mkdir -p "${out_dir}"

cd "${work}/${pkgdir_arg}"
# PATH explícito: o secure_path do sudo da imagem põe /usr/sbin (link para
# /usr/bin) primeiro, o que confunde a regra de shebang do namcap.
as_builder() { sudo -u builder -H env PATH=/usr/local/bin:/usr/bin "$@"; }

echo "::group::namcap PKGBUILD"
as_builder namcap PKGBUILD | tee "${work}/namcap-PKGBUILD.txt"
echo "::endgroup::"

# A .SRCINFO versionada precisa estar sincronizada com o PKGBUILD.
if ! diff -u .SRCINFO <(as_builder makepkg --printsrcinfo); then
  echo ".SRCINFO desatualizada: rode 'makepkg --printsrcinfo > .SRCINFO'" >&2
  exit 1
fi

# Pacotes cujas dependências de execução só existem nos repositórios
# BigLinux/BigCommunity (que este contêiner não usa) marcam o diretório com
# .ci-nodeps: compilam sem resolver dependências e pulam o teste de
# instalação. Só serve para pacotes "any" cujo build não usa essas deps.
nodeps=0
[[ -f .ci-nodeps ]] && nodeps=1

echo "::group::makepkg"
if ((nodeps)); then
  as_builder makepkg --nodeps --noconfirm --cleanbuild --log
else
  as_builder makepkg --syncdeps --noconfirm --cleanbuild --log
fi
echo "::endgroup::"

# Instala os pacotes no contêiner descartável: serve de teste de instalação
# (dependências resolvíveis) e evita falsos positivos do namcap sobre
# bibliotecas fornecidas por outro pacote do mesmo split.
if ((nodeps)); then
  echo "teste de instalação pulado (.ci-nodeps): dependências fora dos repositórios do contêiner"
else
  echo "::group::instalação"
  pacman -U --noconfirm ./*.pkg.tar.zst
  echo "::endgroup::"
fi

echo "::group::namcap pacotes"
for pkg in ./*.pkg.tar.zst; do
  echo "== ${pkg}"
  as_builder namcap "${pkg}"
done | tee "${work}/namcap-packages.txt"
echo "::endgroup::"

cp ./*.pkg.tar.zst "${work}"/namcap-*.txt "${out_dir}/"
cp ./*.log "${out_dir}/" 2>/dev/null || true
# Pacotes com epoch (ex.: wlmaker-1:0.8...) têm ":" no nome, que o
# upload-artifact recusa. O pacman e o repo-add usam a versão do .PKGINFO,
# não o nome do arquivo: "_" no lugar de ":" não muda nada para eles.
for f in "${out_dir}"/*:*; do
  [[ -e ${f} ]] || continue
  mv -- "${f}" "${out_dir}/$(basename -- "${f}" | tr ':' '_')"
done
echo "Pacotes gerados em ${out_dir}:"
ls -l "${out_dir}"
