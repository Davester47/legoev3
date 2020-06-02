project('legoev3', 'c', version: '0.1A')

# Define the source files
src = ['src/ev3.c', 'src/ev3Btn.c', 'src/ev3In.c', 'src/ev3Out.c', 'src/ev3Snd.c']

# Executable example(s)
exe = 'examples/example.c'

# Common include directory
inc = include_directories('include')

# Headers that need to be installed
hdrs = ['include/ev3.h', 'include/pitches.h']
install_headers(hdrs)

# Build the library. 'egoev3' because 'lib' will be appended on it
ev3 = static_library('egoev3', src, include_directories: inc, install: true,
    install_dir: 'lib', native: false)

# Build the example
executable('example', 'examples/example.c', include_directories: inc,
    link_with: ev3, native: false)

# Build the library for the build machine for testing purposes
test = static_library('test_lib', [src, 'tests/common.c'], include_directories:inc, 
    native: true, c_args: '-DEV3_TEST')

# Each test is ['exename', 'srcfile']
tests = [['sound', 'tests/snd.c'],
         ['buttons', 'tests/btn.c']]
# Build & link the tests
foreach t : tests
    e = executable(t[0], t[1], include_directories: inc, native: true,
        link_with: test)
    test(t[0], e)
endforeach
