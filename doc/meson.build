

doxygen = find_program('doxygen'
                       # , required: get_option('docs')
                      )

DOXYGEN_INPUT=[ meson.project_source_root() / 'doc' ,
                meson.project_source_root() / 'fmd' ,
                meson.project_source_root() / 'fmd' / 'bits' 
              ]


# Get the list of absolute file
#abs_doc_files = []
#foreach file : doc_files
#    abs_doc_files += join_paths(meson.project_source_root() , file)
#endforeach


if doxygen.found()
  conf_data = configuration_data()
  conf_data.set('DOXYGEN_INPUT', ' '.join(DOXYGEN_INPUT))
  conf_data.set('DOXYGEN_OUTPUT', meson.current_build_dir())
  doxyfile = configure_file( input: 'Doxyfile.in',
                             output: 'Doxyfile',
                             configuration: conf_data   )
  custom_target('doc',
                command: [doxygen, doxyfile],
                output: ['html'],
                build_by_default: false,
                build_always_stale: true

               )
endif

