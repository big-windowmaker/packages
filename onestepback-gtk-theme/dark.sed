# Big WindowMaker: OneStepBack-Dark, a variação escura do OneStepBack
# (gtk-3.0/gtk.css e gtk-4.0/gtk.css). O tema usa "três cinzas e uma cor":
# os cinzas passam a cinza-escuros e o texto, que o tema escreve com
# @black_color, ganha uma cor própria (@osb_fg_color) clara. O preto
# continua nas bordas de sombra, e o relevo (luz em cima e à esquerda,
# sombra embaixo e à direita) não muda.
s/^@define-color lighter_color #d8d8d8;$/@define-color lighter_color #2e2e2e;/
s/^@define-color base_color #a8a8a8;$/@define-color base_color #3c3c3c;/
s/^@define-color darker_color #787878;$/@define-color darker_color #686868;/
s/^@define-color black_color #000000;$/&\n@define-color osb_fg_color #e0e0e0;/
s/^@define-color text_selected_fg_color @lighter_color;$/@define-color text_selected_fg_color @white_color;/
s/^@define-color listitem_selected_fg_color @lighter_color;$/@define-color listitem_selected_fg_color @white_color;/
s/^@define-color menuitem_selected_bg_color @lighter_color;$/@define-color menuitem_selected_bg_color #a8a8a8;/
s/^\(@define-color \(in\|out\)set_lighter_color \)shade(@base_color,1.3);$/\1#6a6a6a;/
s/^\(@define-color theme_\(text\|fg\|tooltip_fg\)_color \)@black_color;$/\1@osb_fg_color;/
s/^\([[:space:]]*color:[[:space:]]*\)@black_color;/\1@osb_fg_color;/
s/^\(@define-color \(window\|view\|sidebar\|secondary_sidebar\|card\|dialog\|popover\|thumbnail\)_fg_color \)@black_color;$/\1@osb_fg_color;/
s/^@define-color osb_outline_color @black_color;$/@define-color osb_outline_color #d0d0d0;/
