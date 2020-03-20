lib_LIBRARIES = src/libegoev3.a
src_libegoev3_a_SOURCES = src/ev3.c src/ev3Out.c src/ev3In.c src/ev3Btn.c
src_libegoev3_a_CPPFLAGS = -I$(top_srcdir)/include

include_HEADERS = include/ev3.h
noinst_HEADERS = include/ev3Private.h include/lms2012.h include/bytecodes.h \
  include/lmstypes.h

noinst_PROGRAMS = examples/example
examples_example_SOURCES = examples/example.c
examples_example_CPPFLAGS = -I$(top_srcdir)/include
examples_example_LDADD =  src/libegoev3.a
