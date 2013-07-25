## Process this file with automake to produce Makefile.in

SUBDIRS = soldout

noinst_LIBRARIES = libbypass.a
libbypass_a_SOURCES = element.cpp document.cpp parser.cpp
libbypass_a_CXXFLAGS = -I$(top_srcdir)/src/soldout
libbypass_a_LIBADD = $(top_srcdir)/src/soldout/libsoldout.a
libbypass_a_LIBS = -lsoldout