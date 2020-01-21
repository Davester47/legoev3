lib_LTLIBRARIES = src/libc_output.la
src_libc_output_la_LDFLAGS = -avoid-version
src_libc_output_la_SOURCES = src/c_output.c src/c_output.h src/lms2012.h
src_libc_output_la_SOURCES += src/lmstypes.h src/bytecodes.h
src_libc_output_la_CFLAGS = -DDISABLE_DAISYCHAIN_COM_CALL

lib_LIBRARIES = src/libegoev3.a
src_libegoev3_a_SOURCES = src/ev3.c src/lms2012.h src/lmstypes.h src/bytecodes.h

include_HEADERS = src/ev3.h

noinst_PROGRAMS = examples/example
examples_example_SOURCES = examples/example.c
examples_example_CPPFLAGS = -I$(top_srcdir)/src
examples_example_LDADD = src/libc_output.la src/libegoev3.a
examples_example_LDFLAGS = -rpath /home/root/lms2012/sys/lib
