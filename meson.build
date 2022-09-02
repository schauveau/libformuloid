project('c++ libformuloid', 'cpp',
  version : '1.0.0',
  license : 'MIT',
  meson_version: '>=0.56.0',
  default_options: [
    'cpp_std=c++20',
    'buildtype=minsize',
    'warning_level=2',         
    'werror=false',
  ],
)

add_global_arguments('-Wfatal-errors' , language : 'cpp')

libformuloid = shared_library('fmd',
                        [ 'fmd/fmd.cpp' ,
                          'fmd/pretty.cc',
                          'fmd/basal.cc'
                        ] ,
                        dependencies : [],
                        install : true)

install_headers(
  'fmd/fmd.h',  
  subdir : 'fmd'
)

install_headers(
  'fmd/bits/algorithm.h',
  'fmd/bits/apply.h',
  'fmd/bits/args.h',
  'fmd/bits/bezier.h',
  'fmd/bits/cast.h',
  'fmd/bits/compute.h',
  'fmd/bits/context.h',
  'fmd/bits/cyclic.h',
  'fmd/bits/easing.h',
  'fmd/bits/element.h',
  'fmd/bits/exceptions.h',
  'fmd/bits/func.h',
  'fmd/bits/formuloid.h',
  'fmd/bits/identity.h',
  'fmd/bits/instantiation.h',
  'fmd/bits/join.h',
  'fmd/bits/lerp.h',
  'fmd/bits/let.h',
  'fmd/bits/make.h',
  'fmd/bits/math.h',
  'fmd/bits/morph.h',
  'fmd/bits/node.h',
  'fmd/bits/operators.h',
  'fmd/bits/pretty.h',
  'fmd/bits/scheme.h',
  'fmd/bits/select.h',
  'fmd/bits/support.h',
  'fmd/bits/traits.h',
  'fmd/bits/transit.h',
  'fmd/bits/typename.h',
  'fmd/bits/waves.h',
  subdir : 'fmd/bits'
)


pkg_mod = import('pkgconfig')
pkg_mod.generate(libraries : libformuloid,
                 version : '1.0',
                 name : 'fmd',
                 filebase : 'fmd',
                 description : 'A C++ Library to manipulate unary functions as operations')


# Note: Using '.' as include directory is neede to find <fmd/fmd.h> in subprojects
fmd_dep = declare_dependency(link_with : libformuloid,
                             include_directories : include_directories('.') )

alias_target('build2', [libformuloid] )


subdir('demos')
subdir('tests')
subdir('doc')

