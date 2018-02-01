project(
  'anna', 'c',
  version : '0.0.2',
  default_options: [ 'c_std=gnu99', 'warning_level=2' ],
  meson_version : '>= 0.40.0')


chmod = join_paths(meson.source_root(), 'meson-chmod.sh')

add_global_arguments('-DHAVE_CONFIG_H=1', language: 'c')

config_h = configuration_data()
config_h.set_quoted('THEME_FILE', join_paths(get_option('prefix'), get_option('datadir'), 'anna', 'default.edj'))
config_h.set_quoted('THEME_INSTALL_DIR', join_paths(get_option('prefix'), get_option('datadir'), 'anna'))
config_h.set('PACKAGE_BIN_DIR', join_paths(get_option('prefix'), get_option('bindir')))

spawnyClient = dependency('sp-client', required : false)

if spawnyClient.found() != true
   sp_subproject = subproject('spawny')
   spawnyClient = declare_dependency(
       dependencies : sp_subproject.get_variable('sp_client')
   )
endif

elementary = dependency('elementary')
top_inc = include_directories('.')

subdir('data')
subdir('src')

configure_file(
  output : 'config.h',
  install : false,
  configuration: config_h
)
