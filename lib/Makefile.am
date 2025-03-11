lib_LTLIBRARIES = libautotools_mvp.la

libautotools_mvp_la_CPPFLAGS = -Werror -Wall -pedantic \
		-I$(abs_top_srcdir)/include

if OSX
libautotools_mvp_la_CPPFLAGS += -I$(HOMEBREW_PREFIX)/include
endif

libautotools_mvp_la_SOURCES = uuid.cxx

if LINUX
libautotools_mvp_la_LIBADD = $(INIT_LIBS) -lm
else
libautotools_mvp_la_LIBADD = $(INIT_LIBS)
endif

libautotools_mvp_la_LDFLAGS = -uuid -Wl,-ld_classic -pthread
