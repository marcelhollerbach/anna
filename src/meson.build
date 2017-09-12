sources = [
  'config_screen.c',
  'config_screen.h',
  'main.c',
  'main.h',
  'ui.c',
  'ui.h'
]

executable('anna', sources,
  dependencies: [spawnyClient, elementary],
  include_directories: top_inc,
  install : true,
  install_dir: get_option('bindir')
)
