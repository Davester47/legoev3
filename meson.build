project('legoev3', 'c', version: '0.1A', license: 'GPL3+')

# Define the source files
legoev3_src = ['src/ev3.c', 'src/ev3Btn.c', 'src/ev3In.c', 'src/ev3Out.c', 'src/ev3Snd.c']

# Common include directory
legoev3_inc = include_directories('include')

# Headers that need to be installed
legoev3_hdrs = ['include/ev3.h', 'include/pitches.h']
install_headers(legoev3_hdrs)

# Build the library. 'egoev3' because 'lib' will be prepended to it
legoev3 = static_library('egoev3', legoev3_src, include_directories: legoev3_inc, 
    install: true, install_dir: 'lib', native: false, build_by_default:true)

# Declare dependency object in case someone embeds this into
# another meson project
legoev3_dep = declare_dependency(
    include_directories: legoev3_inc,
    link_with: legoev3,
)

# Build the example
executable('example', 'examples/example.c', include_directories: legoev3_inc,
    link_with: legoev3, native: false, build_by_default:true)

# Build the library for the build machine for testing purposes
test = static_library('test_lib', [legoev3_src, 'tests/common.c'],
    include_directories:legoev3_inc, 
    native: true, c_args: '-DEV3_TEST', build_by_default: false)

# Each test is ['exename', 'srcfile']
legoev3_tests = [['sound', 'tests/snd.c'],
                 ['buttons', 'tests/btn.c']]
# Build & link the tests
foreach t : legoev3_tests
    e = executable(t[0], t[1], include_directories: legoev3_inc, native: true,
        link_with: test, build_by_default: false)
    test(t[0], e)
endforeach

# Generate a pkg-config file
pkg = import('pkgconfig')
pkg.generate(
    libraries: legoev3,
    name: 'legoev3',
    filebase: 'legoev3',
    description: 'A C library for Lego Mindstorms',
    install_dir: 'lib/pkgconfig'
)