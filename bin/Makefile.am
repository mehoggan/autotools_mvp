bin_PROGRAMS = autotools_mvp_runner

#####
# Start of autotools_mvp_runner
#####
autotools_mvp_runner_SOURCES = main.cpp

autotools_mvp_runner_CPPFLAGS = -Werror -Wall -pedantic \
    -I$(abs_top_srcdir)/include

autotools_mvp_runner_LDADD = \
    $(abs_top_builddir)/lib/libautotools_mvp.la
if LINUX
autotools_mvp_runner_LDADD += -lrt
endif

autotools_mvp_runner_LDADD += \
    -luuid

if LINUX
autotools_mvp_runner_LDFLAGS = -lcrypto -pthread
else
autotools_mvp_runner_LDFLAGS = \
		-Wl,-ld_classic \
    -pthread
endif
autotools_mvp_runner_LDADD += -lm

CLEANFILES = *.o
CLEANDIRS = .lib/
