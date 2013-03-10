
module_LTLIBRARIES = libandroid.la

libandroid_la_LDFLAGS = -module -avoid-version $(LD_NO_UNDEFINED_FLAG)

libandroid_la_SOURCES = android.c

INCLUDES = $(XORG_INCS)

AM_CFLAGS = $(DIX_CFLAGS) $(XORG_CFLAGS)

sdk_HEADERS = android.h

EXTRA_DIST = fbpriv.h
