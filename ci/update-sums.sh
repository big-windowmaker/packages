#!/usr/bin/env bash
# Regenera os checksums (updpkgsums) e a .SRCINFO de um diretório de pacote.
# Roda como root dentro do contêiner, com o repositório montado em modo
# leitura e escrita e --userns=keep-id (ver README.md). O makepkg roda com o
# UID do dono do diretório, para os arquivos continuarem sendo do usuário.
set -euo pipefail

pkgdir_arg=${1:?informe o diretório do pacote}
repo_root=$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)

if [[ ! ${pkgdir_arg} =~ ^[a-z0-9][a-z0-9._+-]*$ ]] || [[ ! -f ${repo_root}/${pkgdir_arg}/PKGBUILD ]]; then
  echo "diretório de pacote inválido: ${pkgdir_arg}" >&2
  exit 2
fi

if [[ ! -f /etc/pacman.d/gnupg/trustdb.gpg ]]; then
  pacman-key --init
  pacman-key --populate archlinux manjaro
fi
pacman -Sy --noconfirm --needed base-devel git pacman-contrib sudo >/dev/null

owner_uid=$(stat -c %u "${repo_root}/${pkgdir_arg}/PKGBUILD")
owner=$(getent passwd "${owner_uid}" | cut -d: -f1 || true)
if [[ -z ${owner} ]]; then
  useradd -u "${owner_uid}" -M -d /tmp builder-local
  owner=builder-local
fi

srcdest=$(mktemp -d)
chown "${owner_uid}" "${srcdest}"
cd "${repo_root}/${pkgdir_arg}"
sudo -u "${owner}" env HOME=/tmp SRCDEST="${srcdest}" PATH=/usr/local/bin:/usr/bin \
  bash -c 'updpkgsums && makepkg --printsrcinfo > .SRCINFO'
git -c safe.directory="*" -C "${repo_root}" --no-pager diff --stat -- "${pkgdir_arg}" 2>/dev/null || true
