

# Additional options passed to the compiler when building the tests
test_args = [ '-DSRC_DIR="@0@"'.format(meson.project_source_root()),
              '-DBUILD_DIR="@0@"'.format(meson.project_build_root()),
              '-DDATA_DIR="@0@"'.format(meson.current_source_dir()),
            ]

#
# Simple tests using the 'tap' protocol. 
#
# Each test should consist of a single .cc file with a function
#
#   bool tap_main() 
#
# The api for the tap protocol is provided by "tap.h"
#

tap_tests = [
  't_arith',
  't_constant',
  't_identity',
  't_join_m',
  't_let',
  't_select',
  't_transit_m',
]

foreach name : tap_tests
  test_exe = executable(name,
                        build_by_default: false,
                        sources : [ name+'.cc' , 'tap.cc' ],
                        dependencies: [fmd_dep],
                        cpp_args : test_args, 
                        install : false,
                       )
  test(name,
       test_exe,
       protocol: 'tap',
       is_parallel : false
      )
endforeach


