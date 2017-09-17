bin_edje_cc = find_program('edje_cc')

gen_src = custom_target('gen-output',
  input : ['default.edc'],
  output : ['default.edj'],
  command : [bin_edje_cc, '@INPUT@', '@OUTPUT@'],
  install : true,
  install_dir: join_paths(get_option('prefix'), get_option('datadir'), 'anna')
)

meson.add_install_script(chmod, 'a+r', join_paths(get_option('prefix'), get_option('datadir'), 'anna', 'default.edj'))
