# Big WindowMaker — pacotes

PKGBUILDs dos pacotes próprios da distribuição **Big WindowMaker** (baseada em
BigLinux/BigCommunity/Manjaro). Cada diretório da raiz é um `pkgbase`, com o
`PKGBUILD`, a `.SRCINFO` gerada por `makepkg --printsrcinfo` e, quando houver,
os patches.

| Diretório | Pacotes gerados | Versão | Origem |
|---|---|---|---|
| [`windowmaker/`](windowmaker/PKGBUILD) | `windowmaker` | `0.96.0.r130.g558e67f` | `master` do [window-maker/wmaker](https://github.com/window-maker/wmaker) fixado no commit `558e67f`. Substitui o `extra/windowmaker 0.96.0`. |
| [`wlmaker/`](wlmaker/PKGBUILD) | `wlmaker` | `0.8.1` | Tag `v0.8.1` (branch-0.8) do [phkaeser/wlmaker](https://github.com/phkaeser/wlmaker), com XWayland, contra `wlroots0.20`. |
| [`big-dockapps/`](big-dockapps/PKGBUILD) | `libdockapp`, `wmcpuload`, `wmbattery`, `wmusic`, `wmshutdown`, `wmmoonclock`, `wmcube`, `wmudmount`, `wmnet` | `20260412` | [window-maker/dockapps](https://github.com/window-maker/dockapps) fixado no commit `251e851`; `wmudmount` vem do tarball 3.0 do SourceForge. |
| [`biglinux-livecd/`](biglinux-livecd/PKGBUILD) | `biglinux-livecd` | `26.10.26` | **Temporário**, só para as ISOs de teste: branch `window-maker` de [big-windowmaker/biglinux-livecd](https://github.com/big-windowmaker/biglinux-livecd), com o suporte à sessão live do Window Maker proposto ao upstream. Sai daqui quando a mudança estiver no `biglinux-stable`. |

### Observações

- **windowmaker**: mesmas flags de configure do Arch, mais `--enable-pango` e
  `--enable-res`, e as dependências `libxres`, `libxkbfile`, `libjxl` e
  `libarchive` (ver `analysis/02-x11-wmaker-dockapps.md`, seção 2.3). O
  `check()` roda o `make check` do upstream. As sonames (`libwraster.so.6`,
  `libWINGs.so.3`, `libWUtil.so.5`, `libWMaker.so.1`) são as mesmas da 0.96.0.
- **wlmaker**: os submódulos `libbase` e `inih` entram como fontes separadas,
  fixadas nos commits registrados na v0.8.1, e são ligados em `prepare()` com
  `git config submodule.<nome>.url` (padrão do Arch). O `prepare()` confere o
  commit da tag. O `check()` roda os 12 testes do CTest.
- **big-dockapps**: num pacote split o `makepkg` não permite `pkgver` diferente
  por pacote. Por isso todos usam a data do commit fixado, que é maior que as
  versões do AUR. As versões upstream incluídas estão listadas no topo do
  `PKGBUILD`. A `libdockapp` (que não existe nos repositórios) entra no split
  porque `wmusic` e `wmcube` dependem dela. Ficam de fora os dockapps que já
  vêm do repositório BigLinux (`wmclock`, `wmix`, `wmsystemtray`,
  `wmmemload`). O `wmnet` substitui o `wmnd` do BigLinux, que está ligado a
  uma `libsnmp` que não existe mais.
- **biglinux-livecd**: as dependências de execução só existem nos
  repositórios BigLinux, então o diretório tem `.ci-nodeps`: o CI compila sem
  resolver dependências e pula o teste de instalação.

## Política

1. **Fontes fixadas.** Todo `source` aponta para um commit (`#commit=`) ou para
   uma tag cujo commit é conferido em `prepare()`, ou para um tarball de
   release. Todos têm `sha256sums` reais: o `makepkg` também calcula checksum
   de fontes git. `SKIP` não é aceito.
2. **Sem segredos no build.** O CI de build roda com `permissions: contents:
   read`, sem nenhum segredo, e só gera artefatos. Assinatura e publicação no
   repositório ficam num pipeline separado, com environment protegido.
3. **Actions e imagens fixadas.** Toda `uses:` é fixada por SHA completo, com a
   versão em comentário, e o Dependabot abre PRs de atualização. A imagem
   `manjarolinux/base` é fixada por digest e atualizada manualmente.
4. **makepkg sem root.** O build roda como usuário sem privilégios dentro do
   contêiner. Esse usuário só pode chamar o `pacman` via sudo, para instalar
   as dependências.
5. **namcap obrigatório.** O `namcap` roda no `PKGBUILD` e em cada pacote
   gerado. Os pacotes também são instalados no contêiner descartável, como
   teste de instalação.
6. **Patches pequenos e rastreáveis.** Patches ficam em `<pkgbase>/` e cada um
   deve apontar para o PR correspondente no upstream. Tema e configuração não
   entram como patch: ficam nos pacotes de configuração.

## Compilar localmente com podman

Não é preciso instalar nada no sistema. Basta o `podman` sem root:

```sh
git clone https://github.com/big-windowmaker/packages
cd packages
mkdir -p out/windowmaker

podman run --rm \
  -v "$PWD":/repo:ro \
  -v "$PWD/out/windowmaker":/out \
  docker.io/manjarolinux/base@sha256:bbf1f1d746f28e138eea610e140d2f28cbb5b7c5da2fbff034b883527aa604e9 \
  bash /repo/ci/build-package.sh windowmaker /out
```

Troque `windowmaker` por `wlmaker` ou `big-dockapps`. O script
[`ci/build-package.sh`](ci/build-package.sh) é o mesmo usado no CI. Ele:

1. atualiza o contêiner e instala `base-devel`, `git` e `namcap`;
2. cria o usuário `builder`, sem privilégios;
3. copia o diretório do pacote para dentro do contêiner (o repositório é
   montado só para leitura);
4. roda o `namcap` no `PKGBUILD` e confere se a `.SRCINFO` está em dia;
5. roda `makepkg --syncdeps --cleanbuild` (com `check()`);
6. instala os pacotes gerados e roda o `namcap` em cada um;
7. copia os pacotes, os logs e os relatórios do namcap para `/out`.

O build acontece dentro do sistema de arquivos do contêiner. Isso evita
problemas com `/tmp` montado como `noexec` no host.

## Atualizar um pacote

1. Edite o `PKGBUILD`: commit ou tag, `pkgver`, e `pkgrel=1`.
2. Regenere os checksums e a `.SRCINFO` dentro do contêiner, com
   [`ci/update-sums.sh`](ci/update-sums.sh). O `--userns=keep-id` mantém os
   arquivos com o seu usuário:
   ```sh
   podman run --rm --userns=keep-id --user root -v "$PWD":/repo \
     docker.io/manjarolinux/base@sha256:bbf1f1d746f28e138eea610e140d2f28cbb5b7c5da2fbff034b883527aa604e9 \
     bash /repo/ci/update-sums.sh <pkgbase>
   ```
   Confira no `git diff` se só mudou o que era esperado.
3. Compile como descrito acima e abra um PR. O CI compila só os diretórios
   alterados, ou todos quando `ci/` ou o workflow mudam.

## Licença

Os arquivos deste repositório (PKGBUILDs, scripts e configuração) seguem a
convenção do Arch Linux e estão sob a licença [0BSD](LICENSE). O software
empacotado mantém a licença do seu upstream, registrada no campo `license` de
cada pacote.
