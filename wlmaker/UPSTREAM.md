# Patches do wlmaker e o upstream

O pacote `wlmaker` aplica 179 patches (176 do wlmaker e 3 do `libbase`) sobre o
commit `5576886` do [phkaeser/wlmaker](https://github.com/phkaeser/wlmaker).
Cada rebase custa uma rodada inteira, e uma mudança grande no upstream pode
nos prender a uma versão velha. A meta do
[plano da plataforma](https://github.com/big-windowmaker/.github/blob/main/docs/PLANO.md)
é tirar metade dos patches daqui em 3 meses, mandando ao upstream o que
serve para todo mundo.

Triagem de 2026-11-26, pelo assunto e pelos arquivos de cada patch. É uma
primeira leitura: antes de enviar, cada patch é rebaseado sobre o `main` do
upstream, compilado e testado sozinho.

## 1. Correções genéricas: enviar primeiro

Bugs e robustez que qualquer usuário do wlmaker tem.

| Patches | Assunto |
| --- | --- |
| 0003, 0004, 0024 | `lock_mgr`: novo bloqueio assume um abandonado; grabs e foco ao bloquear; vazamento no teste |
| 0025, 0026, 0034 | `wlclient`: ordem dos timers, espera pelo próximo timer, saída limpa no SIGTERM |
| 0033, 0045 | `dock`: sai quando o compositor some; não esgota os buffers de saída |
| 0037, 0158 | `toolkit`: foco do ponteiro só de uma posição conhecida; um clique mantém o ponteiro até soltar todos os botões |
| 0044, 0073, 0129 | `layer`: área útil recalculada; painéis de borda inteira primeiro; zonas exclusivas de todos os painéis |
| 0060 | `desktop`: destruir a última área de trabalho também quando ela é a atual |
| 0076 | `corner`: cantos inferiores trocados |
| 0085, 0147 | `popups`: dispensar popups com grab; menus do XWayland e `xdg_popup` no lugar certo |
| 0100, 0101, 0102 | renderização por pixman quando o GLES seria software; buffers dos menus |
| 0103 | `keyboard`: atalhos também pela tecla sem modificador de nível |
| 0150, 0151, 0154, 0155, 0156, 0166 | XWayland: diálogos do Steam, saída primária, tela cheia pedida antes de mapear, surface destruída, ponteiro preso ao clique, restaurar |
| 0159 | `server`: ignora pedidos de ativação sem entrada do usuário (roubo de foco) |
| 0160, 0163, 0164, 0165 | `wlmaker`: limite de arquivos, sinais bloqueados em todas as threads, programas do Autostart sem zumbis, SIGTERM como Sair |
| 0161, 0162, 0168 | testes do refactor do layout, avisos do compilador, erros de protocolo no log |
| libbase 0001, 0002 | `subprocess`: fecha os pipes de um processo terminado (close-on-exec); inicializa as saídas no teste |
| libbase 0003 | `plist`: string sem aspas só se for ASCII |

## 2. Protocolos e funcionalidades: conversar antes

Úteis fora do Big WindowMaker, mas são decisões de produto do upstream.

- **Protocolos:** 0001 (xdg-activation), 0002 (decoração KDE), 0006 (presentation-time), 0007 (single-pixel-buffer), 0008 (xdg-foreign), 0009 (ext-idle-notify), 0010 (gamma), 0011, 0012 (relative-pointer), 0013 (virtual pointer), 0014 (virtual keyboard), 0042, 0081 (foreign-toplevel), 0149 (pointer-constraints).
- **Paridade com o Window Maker:** 0019 (sombrear), 0020, 0021 (miniaturizar), 0062–0064 (submenus, rolagem de menus), 0075 (áreas de trabalho por número), 0077 (título justificado), 0078 (aceleração do ponteiro), 0079 (recarregar no SIGHUP), 0080, 0084, 0086, 0092, 0093 (efeitos e comportamento do Dock, gavetas), 0094 (arrastar e soltar), 0126, 0134, 0135, 0171 (menu raiz), 0140 (Alt+Tab), 0174–0186 (Clip em grade).
- **Dockapps nativos:** 0028–0030, 0051, 0067–0071 (`wlmcalclock`, `wlmmixer`, preferências).

## 3. Específicos do Big WindowMaker: ficam aqui

- **Zonas e mosaico (wmake-zones):** 0104, 0106, 0107, 0109, 0113, 0117–0121, 0146, 0170, 0183.
- **Integração com a sessão do big-wm:** 0099, 0105, 0115, 0116, 0184 (`share/`), 0123, 0124, 0130, 0131, 0148 (dockapps do X11 pelo XWayland), 0132 (borda dos dockapps), 0133 (Spectacle), 0145, 0173 (wlmdock).
- **Tradução:** 0047, 0114, 0127.

Os demais (Dock, Clip, configuração, estado) dependem de patches dos grupos 2
e 3 e seguem com eles.

## Andamento

Os cinco primeiros foram rebaseados sobre o `main` do upstream (`5576886`), um commit por branch no fork (`upstream/*` em big-windowmaker/wlmaker), e compilados e testados juntos ali: `ctest` 26/26.


| Patch | Estado |
| --- | --- |
| 0025 | [phkaeser/wlmaker#575](https://github.com/phkaeser/wlmaker/pull/575) (2026-11-26) |
| 0003 | [phkaeser/wlmaker#576](https://github.com/phkaeser/wlmaker/pull/576) |
| 0037 | [phkaeser/wlmaker#577](https://github.com/phkaeser/wlmaker/pull/577) |
| 0044 | [phkaeser/wlmaker#578](https://github.com/phkaeser/wlmaker/pull/578) |
| 0076 | [phkaeser/wlmaker#579](https://github.com/phkaeser/wlmaker/pull/579): correção e funcionalidade juntas; separar se o upstream pedir |
| 0158, 0085 | Aplicam sozinhos; próximos. |
| Demais do grupo 1 | Dependem de outros patches: rebasear um a um. |

Quando um patch entra no upstream, ele sai daqui no próximo
`_commit`.
