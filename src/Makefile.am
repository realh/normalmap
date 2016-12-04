bin_PROGRAMS = normalmap

normalmap_SOURCES = \
	main.c \
	nmpng.c					nmpng.h \
	normalmap.c				normalmap.h \
	options.c				options.h

normalmap_CFLAGS = -I/mingw32/include/libpng16 -I/mingw32/include
normalmap_LDFLAGS = -L/mingw32/lib
normalmap_LDADD = -lpng -lz -lpopt -lintl -liconv -lm
