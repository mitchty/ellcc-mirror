ifeq ($(ARCH),ppc)
    EXCLUDE = "http://ellcc.org/bugzilla/show_bug.cgi?id=20"
endif
ifeq ($(ARCH),ppc64)
    EXCLUDE = "http://ellcc.org/bugzilla/show_bug.cgi?id=20"
endif
INPUT = touch file ;
ARGS = file
OUTPUT = && test ! -e file
