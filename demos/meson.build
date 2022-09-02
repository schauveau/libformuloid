
# To build the example,
#fmd_dep      = dependency('fmd',        required: true)
cairo_dep    = dependency('cairo',      required: false)
sdl2_dep     = dependency('sdl2',       required: false)
libxml_dep   = dependency('libxml-2.0', required: false)


all_demos = {
               'demo-anim':
                {
                  'src': [ 'demo-anim.cc' ],
                  'dep': [ fmd_dep, cairo_dep, sdl2_dep ] ,
                  'cpp_args': [ '-DFOOBAR=1' ]
                },
                'demo-graph':
                {
                  'src': [ 'demo-graph.cc' ],
                  'dep': [ fmd_dep, cairo_dep, sdl2_dep ] ,
                  'cpp_args': [ '-DFOOBAR=1' ]
                },
                'demo-syntax':
                {
                  'src': [ 'demo-syntax.cc' ],
                  'dep': [ fmd_dep ] ,
                  'cpp_args': [ '-DFOOBAR=1' ]
                }
            }

# Additional options passed to all demos
# project_ --> global_ ???
demo_cpp_args = [ '-DSRC_DIR="@0@"'.format(meson.project_source_root()),
                  '-DBUILD_DIR="@0@"'.format(meson.project_build_root()),
                  '-DDATA_DIR="@0@"'.format(meson.current_source_dir()),
                ] 


foreach name, info : all_demos
  ok = true
  foreach d : info.get('dep')
    if not d.found()
      ok = false
    endif
  endforeach
  if ok
    executable(name,
               sources : info.get('src'),
               dependencies: info.get('dep'),
#               build_by_default: get_option('demos'),
               cpp_args : demo_cpp_args + info.get('cpp_args'),
               install : false
              )
  else
    warning('Ignoring demo ' + name + ' because of missing dependencies')
  endif
endforeach

