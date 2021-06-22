project('Asteroids', 'c')

compiler = meson.get_compiler('c')

csfml_dir = meson.current_source_dir() + '/lib/CSFML/lib/gcc'

dependencies = [
    compiler.find_library('csfml-system', dirs: csfml_dir, static: true),
    compiler.find_library('csfml-window', dirs: csfml_dir, static: true),
    compiler.find_library('csfml-graphics', dirs: csfml_dir, static: true)
]

# All the source files that end in '.c' in alphabetical order.
source = files([
    'src/main.c',
    'src/util/list.c'
])

include = [
    include_directories('src'),
    include_directories('lib/CSFML/include')
]

add_project_arguments(
    '-Wall',
    language : 'c'
)

# Copy DLL files for now
# dlls = [
#     'csfml-audio-2.dll',
#     'csfml-graphics-2.dll',
#     'csfml-network-2.dll',
#     'csfml-system-2.dll',
#     'csfml-window-2.dll'
# ]
# if host_machine.system() == 'windows'
#     foreach dll : dlls
#         run_command(
#             'copy',
#             meson.current_source_dir() + '/lib/CSFML/bin/' + dll,
#             meson.current_source_dir() + '/bin/' + dll
#         )
#     endforeach
# endif

# Create the executable from source, linking with the CSFML dependencies.
executable(
    'Asteroids',
    source,
    include_directories: include,
    dependencies: dependencies,
    install: false
)
