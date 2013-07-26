## Process this file with automake to produce Makefile.in

SUBDIRS = src test
dist_doc_DATA = README

include aminclude.am

if DX_COND_man

# You'd probably want to post-process man pages and installed the patched
# versions.
dist_man1_MANS = @DX_DOCDIR@/man/man1/doxample.c.1

$(dist_man1_MANS): doxygen-doc

endif

MOSTLYCLEANFILES = $(DX_CLEANFILES)

EXTRA_DIST = cleanup bootstrap $(TESTS) $(DX_CONFIG)
