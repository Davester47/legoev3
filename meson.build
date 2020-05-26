# 'lib' will be prefixed to the project name, -legoev3 will link in GCC
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

# Build the library
ev3 = static_library('egoev3', src, include_directories: inc, install: true,
    install_dir: 'lib', native: false)

# Build the example
executable('example', 'examples/example.c', include_directories: inc,
    link_with: ev3, native: false)


# Build the library for the build machine for testing purposes
test = static_library('test_lib', src, include_directories:inc, native:true,
    c_args: '-DEV3_TEST')


e = executable('snd', 'tests/snd.c', include_directories: inc, native: true,
    link_with: test)
test('ev3Snd.c', e)
