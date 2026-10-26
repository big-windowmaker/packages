<p align="center">
  <img src="https://raw.githubusercontent.com/big-windowmaker/big-wm/main/themes/logo/big-wm.svg" width="110" alt="Logo do Big WindowMaker">
</p>

<h1 align="center">Big WindowMaker — pacotes</h1>

<p align="center">
  <a href="https://github.com/big-windowmaker/packages/actions/workflows/build.yml"><img src="https://github.com/big-windowmaker/packages/actions/workflows/build.yml/badge.svg" alt="build"></a>
  <img src="https://img.shields.io/badge/PKGBUILDs-0BSD-blue" alt="Licença 0BSD">
</p>

PKGBUILDs dos pacotes próprios da distribuição **Big WindowMaker** (baseada em
BigLinux/BigCommunity/Manjaro). Cada diretório da raiz é um `pkgbase`, com o
`PKGBUILD`, a `.SRCINFO` gerada por `makepkg --printsrcinfo` e, quando houver,
os patches. Todo push em `main` compila os pacotes alterados num contêiner
Manjaro limpo, roda `check()`, instala os pacotes e passa o `namcap`; os
pacotes ficam disponíveis como artefatos por 30 dias e são usados pelo
[build-iso](https://github.com/big-windowmaker/build-iso) e pelos testes de
integração do [big-wm](https://github.com/big-windowmaker/big-wm).

| Diretório | Pacotes gerados | Versão | Origem |
|---|---|---|---|
| [`windowmaker/`](windowmaker/PKGBUILD) | `windowmaker` | `0.96.0.r130.g558e67f` | `master` do [window-maker/wmaker](https://github.com/window-maker/wmaker) fixado no commit `558e67f`. Substitui o `extra/windowmaker 0.96.0`. |
| [`wlmaker/`](wlmaker/PKGBUILD) | `wlmaker` | `1:0.8.r80.g7481afe` | `main` (desenvolvimento da 0.9) do [phkaeser/wlmaker](https://github.com/phkaeser/wlmaker) fixado no commit `7481afe`, com 41 patches da branch `big-wm/protocols` do [fork](https://github.com/big-windowmaker/wlmaker), XWayland, contra `wlroots0.20`. |
| [`big-dockapps/`](big-dockapps/PKGBUILD) | `libdockapp`, `wmcpuload`, `wmbattery`, `wmusic`, `wmshutdown`, `wmmoonclock`, `wmcube`, `wmudmount`, `wmnet`, `wmclock`, `wmmemload`, `wmsystemtray`, `pclock`, `wmwifi`, `minidock`, `minicontrol`, `wmapmload`, `mountapp` | `20260412` | [window-maker/dockapps](https://github.com/window-maker/dockapps) fixado no commit `251e851`; `wmudmount` (3.0), `wmsystemtray` (1.4) e `mountapp` (3.0) vêm dos tarballs do SourceForge; `minidock` (1.9) e `minicontrol` (0.2) da Zenwalk; `wmapmload` (0.3.4) do site do autor. |
| [`biglinux-livecd/`](biglinux-livecd/PKGBUILD) | `biglinux-livecd` | `26.10.26` | **Temporário**, só para as ISOs de teste: branch `window-maker` de [big-windowmaker/biglinux-livecd](https://github.com/big-windowmaker/biglinux-livecd), com o suporte à sessão live do Window Maker proposto ao upstream. Sai daqui quando a mudança estiver no `biglinux-stable`. |
| [`gtk-nocsd/`](gtk-nocsd/PKGBUILD) | `gtk-nocsd` | `4.8` | Release 4.8 do [MorsMortium/gtk-nocsd](https://codeberg.org/MorsMortium/gtk-nocsd) (Codeberg). Tira a CSD das janelas GTK 3/4 e libadwaita; carregado pela sessão X11 do big-wm. |

### Observações

- **windowmaker**: mesmas flags de configure do Arch, mais `--enable-pango` e
  `--enable-res`, e as dependências `libxres`, `libxkbfile`, `libjxl` e
  `libarchive` (ver `analysis/02-x11-wmaker-dockapps.md`, seção 2.3). O
  `check()` roda o `make check` do upstream. As sonames (`libwraster.so.6`,
  `libWINGs.so.3`, `libWUtil.so.5`, `libWMaker.so.1`) são as mesmas da 0.96.0.
  Um patch próprio faz o `setstyle` manter os caminhos absolutos das
  texturas dos temas (antes ele prefixava o diretório do tema e o papel de
  parede não carregava); a enviar ao upstream.
- **wlmaker**: snapshot do `main` (0.9 em desenvolvimento: `wlmdock` como
  binário separado, `ext-foreign-toplevel-list-v1`, `ext-image-copy-capture-v1`,
  `cursor-shape-v1`, correções de vazamentos e travamentos). O `pkgver` segue o
  `git describe` (conferido em `prepare()`) e leva `epoch=1`, porque
  `0.8.r80` < `0.8.1` para o `vercmp`. Os submódulos `libbase` e `inih` entram
  como fontes separadas, fixadas nos commits registrados no `7481afe`, e são
  ligados em `prepare()` com `git config submodule.<nome>.url` (padrão do
  Arch). Nova dependência de build: `librsvg` (exigida pelo CMake, usada só
  pelos exemplos). O `check()` roda os 16 testes do CTest.
  Os patches (`git format-patch` da branch `big-wm/protocols`, um commit por
  mudança, a enviar ao upstream) adicionam os protocolos `xdg-activation-v1`,
  `org_kde_kwin_server_decoration` (decoração do wlmaker nas janelas GTK3),
  `presentation-time`, `single-pixel-buffer-v1`, `xdg-foreign` v1/v2,
  `ext-idle-notify-v1`, `wlr-gamma-control`, `wlr-output-power-management`,
  `relative-pointer`, `wlr-virtual-pointer` e `virtual-keyboard`; corrigem o
  bloqueio de tela (sem foco de teclado quando um menu de bandeja estava
  aberto; bloqueio abandonado sem recuperação), o menu de clique direito que
  disparava o item sob o ponteiro num clique rápido e os dockapps que fechavam
  sem ponteiro no seat; e acrescentam a opção `--lazy_xwayland`.
  Também aproximam as janelas do Window Maker: duplo clique na barra de
  título enrola a janela (shade), "Miniaturize" funciona (botão, menu da
  janela e pedido do cliente; a janela volta pelo Alt+Tab ou ativação) e as
  janelas XWayland ganham o mesmo menu da janela.
  No Clip, `Clip.Dockapps` define a ordem dos dockapps (pelo nome do
  executável), que podem ser arrastados com Alt; a ordem fica salva em
  `~/.config/wlmaker/Dockapps.plist`. `Clip.Launchers` (mesmo formato dos
  lançadores do Dock) prende lançadores logo abaixo do Clip, mostrados por
  um segundo `wlmdock --clip`. O `AppID` dos lançadores é opcional (os
  `State.plist` da 0.8 não o tinham, e o `wlmdock` saía sem abrir).
  Janelas novas abrem centralizadas na área útil da saída sob o ponteiro
  (sem cobrir Clip, Dock e painéis; `WindowPlacement = Center`, como no
  Window Maker), e maximizar respeita a mesma área. O `wlmdock` e os
  dockapps saem limpos no logout (SIGTERM ou fim do compositor), sem core
  dump.
  Clique direito no Clip, num lançador do Dock ou do Clip e num dockapp (com
  Alt, ou na borda do quadrado) abre menus como os do Window Maker. Dock e
  Clip se arrastam para outro canto da tela (nunca os dois no mesmo canto),
  e a posição fica gravada em `~/.config/wlmaker/State.plist`. O `wlmdock`
  desenha com pixman por padrão (sem `WLR_RENDERER`), o que o faz aparecer
  também em VM e GPU onde o renderizador automático falhava.
  Na 0.9 não há mais dock embutido e o `Config.plist` padrão não inicia o
  `wlmdock`: a configuração da sessão precisa colocá-lo (e o
  `wlmdock --clip`, se houver `Clip.Launchers`) no `Autostart`.
  Os `.desktop` dos dockapps e do `wlmdock` ganham `OnlyShowIn=wlmaker;`: na
  sessão X11 eles não abrem e apareciam no menu e no rofi.
- **big-dockapps**: num pacote split o `makepkg` não permite `pkgver` diferente
  por pacote. Por isso todos usam a data do commit fixado, que é maior que as
  versões do AUR. As versões upstream incluídas estão listadas no topo do
  `PKGBUILD`. A `libdockapp` (que não existe nos repositórios) entra no split
  porque `wmusic` e `wmcube` dependem dela. `wmclock`, `wmmemload` e `wmsystemtray` também
  entram: o servidor atual da BigLinux (`packages.biglinux.com.br`) não os
  tem mais, só o espelho antigo. Só o `wmix` continua vindo do BigLinux. O
  `wmnet` substitui o `wmnd`, ligado a uma `libsnmp` que não existe mais.
  `minidock`, `minicontrol`, `wmapmload` e `mountapp` não têm mais
  manutenção upstream e levam patches: `minidock` (GTK 2) e `minicontrol`
  (GTK 1.2) foram portados para GTK 3; `wmapmload` lê a bateria do
  `/sys/class/power_supply` em vez do `/proc/apm`; `mountapp` compila sem o
  configurador `mount.conf` (GTK 1.2) e só mostra os pontos de montagem
  `user` do `/etc/fstab` (para mídia removível, use o `wmudmount`). Os
  comandos padrão de `minidock` e `minicontrol` (`big-wm-open`, `WPrefs`,
  `systemctl poweroff`) ficam em `minidock.cf` e `minicontrol.cf`, fora dos
  patches.
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
