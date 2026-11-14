<p align="center">
  <img src="https://raw.githubusercontent.com/big-windowmaker/big-wm/main/themes/logo/big-wm.svg" width="110" alt="Logo do Big WindowMaker">
</p>

<h1 align="center">Big WindowMaker — pacotes</h1>

<p align="center">
  <b>As receitas (PKGBUILDs) do Window Maker, do wlmaker, dos dockapps, da tela de login e dos temas do Big WindowMaker.</b>
</p>

<p align="center">
  <a href="https://github.com/big-windowmaker/packages/actions/workflows/build.yml"><img src="https://github.com/big-windowmaker/packages/actions/workflows/build.yml/badge.svg" alt="build"></a>
  <img src="https://img.shields.io/badge/receitas-13-2d85c6" alt="13 receitas">
  <img src="https://img.shields.io/badge/pacotes-46-2d85c6" alt="46 pacotes">
  <img src="https://img.shields.io/badge/PKGBUILDs-0BSD-blue" alt="Licença 0BSD">
</p>

<p align="center">
  <a href="#pacotes">Pacotes</a> ·
  <a href="#dependências-entre-as-receitas">Dependências</a> ·
  <a href="#como-o-ci-publica-os-pacotes">CI e artefatos</a> ·
  <a href="#instalar-os-pacotes">Instalar</a> ·
  <a href="#compilar">Compilar</a> ·
  <a href="#atualizar-um-pacote">Atualizar</a> ·
  <a href="#observações-por-pacote">Observações</a> ·
  <a href="#política">Política</a> ·
  <a href="#contribuir">Contribuir</a> ·
  <a href="#créditos">Créditos</a> ·
  <a href="#in-english">English</a>
</p>

PKGBUILDs dos pacotes próprios da distribuição **Big WindowMaker** (baseada em
BigLinux/BigCommunity/Manjaro). Cada diretório da raiz é um `pkgbase`, com o
`PKGBUILD`, a `.SRCINFO` gerada por `makepkg --printsrcinfo` e, quando houver,
os patches. Todo push em `main` compila os pacotes alterados num contêiner
Manjaro limpo, roda `check()`, instala os pacotes e passa o `namcap`; os
pacotes ficam disponíveis como artefatos por 30 dias e são usados pelo
[build-iso](https://github.com/big-windowmaker/build-iso) e pelos testes de
integração do [big-wm](https://github.com/big-windowmaker/big-wm).

<p align="center">
  <img src="docs/screenshots/dockapps.png" alt="Dezesseis dockapps do big-dockapps lado a lado: wmclock, wmcpuload, wmmemload, wmnet, wmcube, wmglobe, wmmoonclock, wmtime, wmeyes, wmmon, wmtz, pclock, wmclockmon, wmcalc, wmpager e wmsun"><br>
  <sub>Dockapps do <code>big-dockapps</code> no Window Maker: wmclock, wmcpuload,
  wmmemload, wmnet, wmcube, wmglobe, wmmoonclock, wmtime, wmeyes, wmmon, wmtz,
  pclock, wmclockmon, wmcalc, wmpager e wmsun (Xvfb, 14/11/2026).</sub>
</p>

## Pacotes

Versões lidas das `.SRCINFO` desta revisão (`pkgver-pkgrel`).

| Diretório | Pacotes gerados | Versão | O que traz | Origem e patches |
|---|---|---|---|---|
| [`windowmaker/`](windowmaker/PKGBUILD) | `windowmaker` | `0.96.0.r130.g558e67f-26` | O Window Maker da sessão X11, com Pango, XRes, JPEG XL e catálogos pt_BR completos. Substitui o `extra/windowmaker` 0.96.0 (o perfil da ISO o fixa com `IgnorePkg`). | `master` do [window-maker/wmaker](https://github.com/window-maker/wmaker) no commit `558e67f`; **31 patches** da branch `big-wm/missao-2026-11` do [fork](https://github.com/big-windowmaker/wmaker): Dock no monitor escolhido e sempre visível, dockapps acoplados que iniciam com a sessão e se arrastam, `_NET_WORKAREA` pelos struts, dicas do Motif depois do mapeamento, Info Panel do Big WindowMaker, fundo redesenhado quando um monitor entra ou sai (0030), moldura de janela sem decoração (0031). |
| [`wlmaker/`](wlmaker/PKGBUILD) | `wlmaker` | `1:0.8.r80.g7481afe-36` | O compositor Wayland da sessão Wayland, com `wlmdock`, Clip, dockapps nativos (`wlmclock`, `wlmmixer`, `wlmeyes`...) e XWayland. | `main` (0.9 em desenvolvimento) do [phkaeser/wlmaker](https://github.com/phkaeser/wlmaker) no commit `7481afe`, contra `wlroots0.20`; **139 patches** das branches `big-wm/*` do [fork](https://github.com/big-windowmaker/wlmaker) e 1 do `libbase`: protocolos (xdg-activation, decoração KDE, presentation-time, idle-notify, gamma, virtual keyboard/pointer, pointer constraints na 0149...), zonas do `wmake-zones` (0104-0121), dockapps do X11 no Dock e no Clip (0122-0131), Alt+Tab, menus e popups no lugar (0147). |
| [`big-dockapps/`](big-dockapps/PKGBUILD) | `libdockapp` e 33 dockapps: `wmclock`, `wmcpuload`, `wmmemload`, `wmnet`, `wmsystemtray`, `wmbattery`, `wmusic`, `wmshutdown`, `wmmoonclock`, `wmcube`, `wmudmount`, `pclock`, `wmwifi`, `minidock`, `minicontrol`, `wmapmload`, `mountapp`, `wmcalclock`, `wmtime`, `wmclockmon`, `wmtz`, `wmtop`, `wmfsm`, `wmmon`, `wmcliphist`, `wmgtemp`, `wmsun`, `wmcalc`, `wmpager`, `wmcpufreq`, `wmglobe`, `wmweather`, `wmeyes` | `20260412-14` | Os dockapps clássicos do Dock e de Aparência > Dockapps. | [window-maker/dockapps](https://github.com/window-maker/dockapps) no commit `251e851` e tarballs de SourceForge, Zenwalk, Debian e FreeBSD; **20 patches**: `wmcliphist` no histórico do `big-wm-clipboard` (Super+V) e com a última cópia certa quando o dono some, `wmusic` seguindo o player MPRIS que toca, `minidock`/`minicontrol` em GTK 3, `wmapmload` pelo `/sys`, `wmcpufreq` pela `libcpupower`... |
| [`wmpulsemixer/`](wmpulsemixer/PKGBUILD) | `wmpulsemixer` | `0.2.2-1` | O volume padrão do Dock: saída e microfone pelo PulseAudio/PipeWire. | Tag `0.2.2` do [polletfa/wmpulsemixer](https://github.com/polletfa/wmpulsemixer); 1 patch: espera o servidor de áudio, roda do mouse pelo volume real, menu de dispositivos sem efeito nos controles. |
| [`wmake-zones/`](wmake-zones/PKGBUILD) | `wmake-zones` | `0.4.1-1` | Zonas de encaixe e mosaico de janelas nas duas sessões: serviço X11, dockapps, preferências e editor (GTK 3) e o núcleo que o `wlmaker` liga. | Tag `v0.4.1` do [big-windowmaker/wmake-zones](https://github.com/big-windowmaker/wmake-zones), sem patches. |
| [`wdm/`](wdm/PKGBUILD) | `wdm` | `1.28-14` | A tela de login da live e do sistema instalado: WINGs Display Manager, visual NeXT, sessões X11 e Wayland, lista de usuários. | Tarball 1.28 e os 11 patches do Debian 1.28-27 (snapshot.debian.org); **5 patches** próprios: GCC 15, sessões X11/Wayland com logind, seletor de sessão e pt_BR, mensagens do PAM e do faillock, lista de usuários. |
| [`gtk-nocsd/`](gtk-nocsd/PKGBUILD) | `gtk-nocsd` | `4.8-2` | Tira a decoração do cliente (CSD) das janelas GTK 3/4 e libadwaita; carregado pela sessão X11. | Release 4.8 do [MorsMortium/gtk-nocsd](https://codeberg.org/MorsMortium/gtk-nocsd); 1 patch: desligado dentro do Steam. |
| [`onestepback-gtk-theme/`](onestepback-gtk-theme/PKGBUILD) | `onestepback-gtk-theme` | `0.997-6` | Tema GTK 2/3/4 NeXTSTEP da sessão e a variação `OneStepBack-Dark`, com as classes do libadwaita. | Zip da 0.997 do [site do autor](http://www.vide.memoire.free.fr/pages/onestepback/onestepback.php); ajustes por `sed` e CSS no build. |
| [`qtstep-kvantum/`](qtstep-kvantum/PKGBUILD) | `qtstep-kvantum` | `0.r23.36dc0d3-2` | Temas NeXTSTEP do Kvantum para os aplicativos Qt, mais o `QTStepOpenStepDark`. | [andbgr/QTStep](https://github.com/andbgr/QTStep) no commit `36dc0d3`. |
| [`retrothemes-firefox/`](retrothemes-firefox/PKGBUILD) | `retrothemes-firefox` | `0.r68.dfc4eea-2` | Temas retrô do Firefox (`userChrome.css`): Netscape 4, Internet Explorer 5-8. | [matthewmx86/RetroThemesFirefox](https://github.com/matthewmx86/RetroThemesFirefox) no commit `dfc4eea`; 1 patch para o Firefox 158. |
| [`thunar-git/`](thunar-git/PKGBUILD) | `thunar-git` | `4.21.7+r139+g222d036c8-1` | O gerenciador de arquivos padrão; `provides`/`conflicts`/`replaces` `thunar`, então os plugins continuam valendo. | `master` do [Thunar](https://gitlab.xfce.org/xfce/thunar) no commit `222d036`, a partir do PKGBUILD do AUR (twa022). |
| [`libxfce4ui/`](libxfce4ui/PKGBUILD) | `libxfce4ui` | `4.21.9-1` | A série 4.21 que o `thunar-git` pede (`>= 4.21.8`); mesmo soname da 4.20 do Manjaro, que substitui. | Tag `libxfce4ui-4.21.9` do [Xfce](https://gitlab.xfce.org/xfce/libxfce4ui), fixada no commit. |
| [`biglinux-livecd/`](biglinux-livecd/PKGBUILD) | `biglinux-livecd` | `26.10.26-13` | **Temporário**, só para as ISOs de teste: sessão live, assistente e Calamares com o perfil `bigwindowmaker` (NeuroDefender, visual do Window Maker, wdm). Sai daqui quando a mudança estiver no `biglinux-stable`. | `main` do [big-windowmaker/biglinux-livecd](https://github.com/big-windowmaker/biglinux-livecd) fixado no commit `b81b6e9`. |

## Dependências entre as receitas

Algumas receitas precisam de um pacote deste repositório, e não do Manjaro,
para compilar. O CI compila e instala esses antes, pelo arquivo
`.ci-localdeps` do diretório; ao compilar à mão, siga a mesma ordem.

| Receita | Compila antes (`.ci-localdeps`) | Por quê |
|---|---|---|
| `wmake-zones` | `windowmaker` | o serviço X11 e os testes rodam contra o nosso Window Maker |
| `wlmaker` | `wmake-zones` | liga no núcleo estático das zonas (`wmake-zones>=0.3.0`) |
| `big-dockapps` | `windowmaker` | `wmglobe` e `mountapp` ligam na `libwraster`/`libWINGs`, cujas versões de símbolo diferem das do Manjaro |
| `wdm` | `windowmaker` | o `wdmLogin` liga na `libWINGs` do nosso Window Maker |
| `thunar-git` | `libxfce4ui` | pede `libxfce4ui >= 4.21.8` |
| `biglinux-livecd` | — (`.ci-nodeps`) | as dependências de execução só existem nos repositórios BigLinux: o CI compila sem resolvê-las e pula o teste de instalação |

Ordem completa: `windowmaker` → `wmake-zones` → `wlmaker`; `windowmaker` →
`big-dockapps` e `wdm`; `libxfce4ui` → `thunar-git`. O `big-dockapps` também
precisa das macros do `xorg-font-util` (`makedepends`); fora do contêiner,
se o `aclocal` não as achar, aponte `ACLOCAL_PATH` para o
`/usr/share/aclocal` onde ele está.

## Como o CI publica os pacotes

```mermaid
flowchart LR
  U[upstream fixado por commit<br/>window-maker/wmaker, phkaeser/wlmaker, dockapps...] --> P
  PT[patches do Big WindowMaker<br/>um por mudança, com descrição] --> P
  P[PKGBUILD + .SRCINFO] --> CI[CI: contêiner Manjaro limpo<br/>makepkg, check, namcap, instalação]
  CI --> A[(artefatos packages-&lt;diretório&gt;<br/>30 dias)]
  A --> ISO[build-iso: ISO de teste]
  A --> IT[big-wm: testes de integração]
```

O workflow [`build.yml`](.github/workflows/build.yml) roda em PR, em push no
`main` e por `workflow_dispatch`:

1. **Detectar** compara com o commit anterior (`before` do push ou base do
   PR) e escolhe só os diretórios alterados; mudanças em `ci/` ou no
   workflow recompilam todos.
2. **Build** (um job por diretório, contêiner `manjarolinux/base` fixado
   por digest) roda [`ci/build-package.sh`](ci/build-package.sh): `namcap`
   no `PKGBUILD`, confere a `.SRCINFO`, compila as `.ci-localdeps`, baixa e
   confere as fontes, `makepkg --syncdeps --cleanbuild` com `check()`,
   instala os pacotes e roda o `namcap` em cada um.
3. Publica `out/` (pacotes, logs, relatórios do namcap) como o artefato
   **`packages-<diretório>`**, por 30 dias.

O build-iso usa só artefatos de runs de **push em `main`** concluídos com
sucesso: o de cada diretório vem do último run que o compilou, então um PR
só chega à ISO depois do merge e do run do `main`. Em PR, um push novo
cancela o run anterior (`concurrency`); no `main`, os runs de merges
seguidos não se cancelam.

## Instalar os pacotes

- **Pela ISO** (recomendado): o Big WindowMaker já vem completo na
  [ISO de teste](https://github.com/big-windowmaker/build-iso/releases).
- **Num BigLinux/BigCommunity já instalado**: os pacotes ainda não estão num
  repositório público assinado. Compile e instale cada um pelo PKGBUILD
  (como usuário comum, nunca como root):

  ```sh
  git clone https://github.com/big-windowmaker/packages
  cd packages/wmake-zones        # ou windowmaker, wlmaker, big-dockapps...
  makepkg -si                    # baixa as dependências, compila, roda o check() e instala
  ```

  Ordem quando for compilar vários: a de
  [Dependências entre as receitas](#dependências-entre-as-receitas)
  (`windowmaker` → `wmake-zones` → `wlmaker`, `libxfce4ui` → `thunar-git`).
  O desktop em si (sessões, temas, ferramentas) vem do
  [big-wm](https://github.com/big-windowmaker/big-wm).
- **Pacotes já compilados**: cada execução do CI em `main` deixa os pacotes
  como artefatos por 30 dias (*Actions* → *build* → *Artifacts*, ou
  `gh run download <run> -R big-windowmaker/packages -n packages-<diretório>`);
  instale com `sudo pacman -U arquivo.pkg.tar.zst`.
- **Voltar atrás**: `sudo pacman -U` com o pacote anterior (o makepkg o deixa
  no diretório do PKGBUILD) ou `sudo pacman -S windowmaker` para o pacote da
  distribuição, quando existir.

## Compilar

### No contêiner do CI (podman)

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

Troque `windowmaker` pelo diretório desejado. O script
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

### Direto no sistema (makepkg)

Num Manjaro/BigLinux, como usuário comum, com os diretórios de trabalho fora
do checkout:

```sh
cd packages/wdm
w=~/.cache/bwm-build
BUILDDIR=$w/build PKGDEST=$w/pkg SRCDEST=$w/src makepkg -s
namcap PKGBUILD "$w"/pkg/*.pkg.tar*
```

Com `.ci-localdeps`, compile e instale antes o pacote listado. Para o
`big-dockapps`, se o `aclocal` reclamar das macros `XORG_FONT_*`, instale o
`xorg-font-util` (ou extraia o pacote e use `ACLOCAL_PATH=<dir>/usr/share/aclocal`).

## Atualizar um pacote

1. Edite o `PKGBUILD`: commit ou tag e `pkgver`. Volte o `pkgrel` para 1
   só quando o `pkgver` mudar; se ele continua igual (mudança só na receita,
   ou um commit novo do `biglinux-livecd`, cujo `pkgver` é fixo), suba o
   `pkgrel`, senão o pacote novo fica mais velho que o instalado.
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

## Observações por pacote

- **windowmaker**: mesmas flags de configure do Arch, mais `--enable-pango` e
  `--enable-res`, e as dependências `libxres`, `libxkbfile`, `libjxl` e
  `libarchive` (ver `analysis/02-x11-wmaker-dockapps.md`, seção 2.3). O
  `check()` roda o `make check` do upstream. As sonames (`libwraster.so.6`,
  `libWINGs.so.3`, `libWUtil.so.5`, `libWMaker.so.1`) são as mesmas da 0.96.0.
  Os 31 patches (`git format-patch` da branch `big-wm/missao-2026-11` do
  fork, um commit por mudança) vão do `setstyle` que mantém os caminhos
  absolutos das texturas (0001) ao Dock por monitor, aos dockapps acoplados,
  ao `IgnoreDecorationChanges` (0020-0021), aos catálogos de todos os
  domínios em pt_BR (0024-0027) e ao RandR (0030); o que servir a todos
  segue para o upstream.
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
  Como no Window Maker, o Dock tem um quadrado principal (logo do Big
  WindowMaker) que não pode ser removido: o menu dele muda o Dock de lado e
  restaura os lançadores padrão (os do `State.plist` do big-wm); o Clip
  também restaura os seus. Os menus aparecem em português.
  No dockapp de volume (`wlmmixer`), as barras coloridas acendem na
  proporção do volume, e clicar ou arrastar sobre elas ajusta o volume
  naquele ponto (no meio, 50%).
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
  A segunda leva (`wmcalclock`, `wmtime`, `wmclockmon`, `wmtz`, `wmtop`,
  `wmfsm`, `wmmon`, `wmcliphist`, `wmgtemp`, `wmsun`, `wmcalc`, `wmpager`,
  `wmcpufreq`, `wmglobe` e `wmweather`) traz os dockapps mais conhecidos que
  faltavam: relógios e calendário, relógio mundial, processos, discos,
  temperatura, área de transferência, calculadora, paginador, frequência da
  CPU, globo e clima. Só três levam patch: `wmcpufreq` passa da
  `libcpufreq` (que não existe mais) para a `libcpupower` do `cpupower`,
  `wmglobe` compila com `-Werror=format-security` e `wmweather` passa `long`
  ao `curl_easy_setopt()`. O `wmglobe` liga na
  `libwraster`, cujas versões de símbolo diferem entre o nosso `windowmaker`
  e o do Manjaro: por isso o diretório tem `.ci-localdeps` com `windowmaker`.
  O `/etc/wmtzrc` do `wmtz` (arquivo `wmtzrc`) mostra UTC, Brasília, Lisboa,
  Nova York e Tóquio no lugar das datas julianas do upstream. Precisam de
  configuração ou de hardware: `wmweather` (código ICAO do aeroporto, ex.
  `wmweather -s SBGR -m`, e rede), `wmsun` (`-lat`/`-lon`, longitude
  positiva a oeste) e `wmcpufreq` (driver cpufreq: não abre em VM sem ele).
  O `wmusic` leva um patch próprio: segue o player MPRIS que está tocando (o
  primeiro da lista, muitas vezes o Firefox parado, escondia o player de
  verdade) e não congela mais quando o comando do player (`-c`, `xmms` por
  padrão) não abre um player MPRIS.
- **biglinux-livecd**: as dependências de execução só existem nos
  repositórios BigLinux, então o diretório tem `.ci-nodeps`: o CI compila sem
  resolver dependências e pula o teste de instalação.
- **onestepback-gtk-theme**: o `prepare()` ajusta só o `gtk-4.0/gtk.css`.
  Apaga a cor `text_view_bg`, para os terminais VTE (AshyTerm) ficarem
  translúcidos, e acrescenta o [`libadwaita.css`](onestepback-gtk-theme/libadwaita.css):
  com `GTK_THEME` definido o libadwaita não carrega a folha de estilo dele,
  e os aplicativos que usam `@window_bg_color`, `@popover_bg_color` ou
  `var(--window-bg-color)` no próprio CSS (Big Audio Player, Big Video
  Player, Big Recorder) abriam com o fundo transparente. O arquivo define
  essas cores e variáveis com os cinzas do tema, sem mudar nenhuma regra
  dele. Também tira o `;;` da linha 155 (`background-color: @darker_color;;`),
  que fazia o GTK 4 avisar "Empty declaration" em todo aplicativo. O
  [`libadwaita-widgets.css`](onestepback-gtk-theme/libadwaita-widgets.css)
  desenha, com o relevo do tema, as classes que só o libadwaita desenhava
  (`.card`, `.boxed-list`, `.pill`, `.suggested-action`, página de status,
  grupos de preferências, avisos, títulos...): sem ele os cartões do
  instalador do BigLinux sumiam.
  O `build()` gera o `OneStepBack-Dark` a partir do tema já ajustado: o
  [`dark.sed`](onestepback-gtk-theme/dark.sed) troca os três cinzas por
  cinza-escuros e dá ao texto (que o tema escreve com `@black_color`) uma
  cor clara própria, sem tocar no preto das sombras do relevo; o
  [`dark-gtk2.sed`](onestepback-gtk-theme/dark-gtk2.sed) faz o mesmo no
  GTK 2; o [`dark.css`](onestepback-gtk-theme/dark.css) acerta a barra de
  título fora de foco; e o ImageMagick reescurece as imagens dos controles
  (setas e caixas invertem, barras de rolagem escurecem). O `check()`
  confere cada troca.
- **wmcliphist** (big-dockapps): cinco patches o integram ao
  `big-wm-clipboard` do big-wm: histórico no banco dele (Super+V), CLIPBOARD
  que cola e UTF-8 (`clipboard-utf8`), a aplicação que copiou continua dona
  da seleção (`keep-the-owner`), só a última cópia volta quando o dono
  some, nunca um favorito (`last-copy`), e na sessão Wayland roda no
  XWayland sem publicar nada (`wayland-clipboard`). O `wmeyes` (1.2, MIT)
  vem do cache de distfiles do FreeBSD, com um patch para o C23.
- **wdm**: o upstream está parado desde 2012; a base é a 1.28 do Debian
  (sessão PAM aberta no processo pai, por causa do logind, entre outros).
  Os arquivos de configuração (`wdm-config`, `wdmLogin-config`, `Xsession`,
  `Wsession`, `wdm.pam`, `wdm.service`...) ficam no diretório, fora dos
  patches; o `check()` compila e roda `test-sessions.c` contra a lista de
  sessões. A ISO usa o `wdm-config` do pacote no sistema instalado e um
  próprio na live (iso-profiles).
- **thunar-git** e **libxfce4ui**: o `thunar-git` é o PKGBUILD do AUR
  fixado num commit da `master` (build reproduzível), com terminal embutido
  (`vte3`), polkit, gexiv2 e gudev e sem a sessão do Xfce. A `master` pede a
  `libxfce4ui` 4.21, que vem daqui com o mesmo soname da 4.20 do Manjaro.
- **gtk-nocsd**: vai no `LD_PRELOAD` da sessão do big-wm. Junto com o
  overlay da Steam, travava o `_v2-entry-point` do Steam Linux Runtime e os
  jogos não abriam; o patch faz a biblioteca se descarregar no lançador da
  Steam e nos processos sob ela, tirando do `LD_PRELOAD` só a si mesma.
- **qtstep-kvantum**: além das 25 variações do QTStep, o
  [`colors.OpenStepDark`](qtstep-kvantum/colors.OpenStepDark) gera pelo
  mesmo modelo o `QTStepOpenStepDark`, o OpenStep em cinza-escuro do modo
  escuro do Big WindowMaker (o `QTStepDark` do projeto tem fundo `#101010`,
  quase preto).

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

## Contribuir

- Correção num pacote: um commit por mudança, com `pkgrel` novo, `.SRCINFO`
  regenerada (`makepkg --printsrcinfo > .SRCINFO`) e checksums atualizados;
  o CI compila o diretório alterado e roda o `namcap`.
- Patch no Window Maker ou no wlmaker: o código vem dos ramos `big-wm/*` dos
  forks ([wmaker](https://github.com/big-windowmaker/wmaker),
  [wlmaker](https://github.com/big-windowmaker/wlmaker)), exportado com
  `git format-patch`; o que servir a todos também segue para o upstream.
- Problemas: [issues](https://github.com/big-windowmaker/packages/issues);
  guia: [CONTRIBUTING](https://github.com/big-windowmaker/.github/blob/main/CONTRIBUTING.md).

## Créditos

### Mantenedores do Big WindowMaker

- **Rafael Ruscher** — <rruscher@gmail.com>
- **Barnabé Di Kartola** — <barnabedikartola@gmail.com>

Nos pacotes de projetos de terceiros, os autores e mantenedores originais
continuam nas linhas `# Contributor:` e no campo `url`; o Big WindowMaker
mantém o empacotamento e os patches de integração.

### Agradecimentos especiais

- **Alfredo Kojima** — pelo [Window Maker](https://www.windowmaker.org/) e
  pelo legado que inspirou este projeto.
- **Philipp Kaeser** — pelo [wlmaker](https://github.com/phkaeser/wlmaker),
  que leva a inspiração do Window Maker ao Wayland.
- **Bruno Gonçalves** — pelo [BigLinux](https://www.biglinux.com.br/) e por
  suas contribuições ao ecossistema Linux.
- **Tales A. Mendonça** — pelas contribuições à
  [BigCommunity](https://communitybig.org).

## Licença

Os arquivos deste repositório (PKGBUILDs, scripts e configuração) seguem a
convenção do Arch Linux e estão sob a licença [0BSD](LICENSE). O software
empacotado mantém a licença do seu upstream, registrada no campo `license` de
cada pacote.

## In English

PKGBUILDs of the Big WindowMaker packages: Window Maker and wlmaker from
pinned upstream commits plus the Big WindowMaker patches, 33 curated
dockapps, wmpulsemixer, wmake-zones, the wdm login screen, Thunar from the Xfce
master (with libxfce4ui 4.21), gtk-nocsd, retro themes and the
live-session/installer package. Every push
builds the changed packages in a clean Manjaro container (makepkg with
`check()`, install test, namcap) and keeps them as CI artifacts used by the
ISO and the integration tests. Build any of them with `makepkg -si` as a
regular user. Maintainers: Rafael Ruscher and Barnabé Di Kartola; upstream
authors stay credited in each PKGBUILD. PKGBUILDs: 0BSD.

