TESTS = gtest_libautotools_mvp
check_PROGRAMS = gtest_libautotools_mvp

gtest_libautotools_mvp_CPPFLAGS = -Werror -Wall -pedantic \
	  -I$(abs_top_srcdir)/include \
	  -I$(abs_top_srcdir)/test/include

if OSX
gtest_libautotools_mvp_CPPFLAGS += -I$(HOMEBREW_PREFIX)/include
endif

gtest_libautotools_mvp_SOURCES = main.cxx \
    test_logging.cxx \
    test_uuid.cxx \
    utils/test_environment.cxx

gtest_libautotools_mvp_LDADD = \
		$(abs_top_builddir)/lib/libautotools_mvp.la

gtest_libautotools_mvp_LDADD += \
    -lgtest -lgmock -luuid -lboost_program_options \
		-lboost_filesystem

if LINUX
gtest_libautotools_mvp_LDADD += \
    -lssl -lcrypto
endif
gtest_libautotools_mvp_LDADD += -lm
gtest_libautotools_mvp_LDFLAGS = -pthread
if OSX
gtest_libautotools_mvp_LDFLAGS += \
		-Wl,-ld_classic
endif

CLEANFILES = *.o
CLEANDIRS = .lib/
