# metalock - enhanced screen locker
# © 2012 Timothy Beyer
# © 2006-2007 Anselm R. Garbe, Sander van Dijk

include config.mk
.include "../themes/${DEFAULT_THEME}/metadata.mk"

.if ${USE_ULTRAGETOPT} == "yes"
OPT_LIBS+=	-lultragetopt
OPT_CPPFLAGS+=	-DULTRAGETOPT_REPLACE_GETOPT
.endif

.if ${USE_IMLIB2} == "yes"
OPT_LIBS+=	-lImlib2
OPT_CPPFLAGS+=	-DWITH_IMLIB2
.endif

.if ${USE_XFT} == "yes"
OPT_LIBS+=	-lXft
OPT_CPPFLAGS+=	-DWITH_XFT
OPT_INCS+=	"-I${LOCALBASE}/include/freetype2"
.endif

# Linux probably needs this
.if ${USE_SHADOW} == "yes"
OPT_CPPFLAGS+=	-DWITH_SHADOW_H
.endif

# This variable does not apply to FreeBSD, it might to other BSDs, though.
.if ${USE_BSD_AUTH} == "yes"
OPT_CPPFLAGS+=	-DWITH_BSD_AUTH
.endif

# On OpenBSD and Darwin disable this, all others most likely need this enabled
.if ${USE_CRYPT} == "yes"
CRYPT=	-lcrypt
.endif

# includes and libs
INCS=	-I. -I${X11INC} -I${LOCALBASE}/include ${OPT_INCS} \
	-I${LOCALBASE}/include/X11
LIBS=	-lc ${CRYPT} -L${X11LIB} -L${LOCALBASE}/lib -lX11 -lXext -lXpm \
	${OPT_LIBS}

# flags
CPPFLAGS=	-DVERSION=\"${VERSION}\" \
			-DPREFIX=\"${PREFIX}\" \
			-DLOCALBASE=\"${LOCALBASE}\" \
			-DEMAIL=\"${EMAIL}\" \
			-DDEFAULT_THEME=\"${DEFAULT_THEME}\" \
			${OPT_CPPFLAGS}

.if ${CC} == "clang"
# -pedantic disabled because of annoying warnings
CFLAGS=	-Wall -Os ${INCS} ${CPPFLAGS}
LDFLAGS=	${LIBS}
.elif ${CC} == "gcc"
# -pedantic disabled because of annoying warning:
# Xfuncproto.h:134:24: warning: ISO C does not permit named variadic macros
CFLAGS=	-std=c99 -Wall -Os ${INCS} ${CPPFLAGS}
LDFLAGS=	-s ${LIBS}
.else
CFLAGS=	-std=c99 -Wall -Os ${INCS} ${CPPFLAGS}
LDFLAGS=	-s ${LIBS}
.endif

SRC=	main.c alloc.c args.c event_loop.c draw_fns.c helper_fns.c
HDR=	data.h alloc.h args.h event_loop.h draw_fns.h helper_fns.h
OBJ=	${SRC:.c=.o}

all: options build postclean

options:
	@echo metalock build options:
	@echo "CFLAGS   = ${CFLAGS}"
	@echo "LDFLAGS  = ${LDFLAGS}"
	@echo "CC       = ${CC}"
	@echo "OBJ      = ${OBJ}"
	@echo

.c.o:
	@echo CC $<
	@${CC} -c ${CFLAGS} $<

${OBJ}: config.mk

prebuild:
	@echo building metalock

postclean:
	@echo "finished building metalock"
	@echo
	@rm -f ${OBJ}

build: prebuild ${OBJ}
	@echo CC -o $@
	@echo "Obj: ${OBJ}"
	@${CC} -o metalock ${OBJ} ${LDFLAGS}

clean:
	@echo cleaning
	@rm -f metalock ${OBJ} metalock-${VERSION}.tar.bz2 \
	metalock_theme-${theme_name}-${theme_version}.tar.bz2

dist: clean
	@echo "creating dist tarball metalock-${VERSION}"
	@mkdir -p metalock-${VERSION}
	@mkdir -p metalock-${VERSION}/src
	@mkdir -p metalock-${VERSION}/themes
	@mkdir -p ../themes
	@cp -p ../LICENSE ../README metalock-${VERSION}
	@cp -pR Makefile config.mk ${SRC} ${HDR} metalock-${VERSION}/src
	@tar -cf metalock-${VERSION}.tar metalock-${VERSION}
	@bzip2 metalock-${VERSION}.tar
	@mv metalock-${VERSION}.tar.bz2 ../dist/
	@rm -rf metalock-${VERSION}

dist-theme: clean
	@echo -n "creating tarball for theme: "
	@echo "metalock_theme-${theme_name}-${theme_version}"
	@mkdir -p ../dist/
	@cd ../themes && \
		tar -cf \
		metalock_theme-${theme_name}-${theme_version}.tar ${theme_name} && \
		bzip2 metalock_theme-${theme_name}-${theme_version}.tar && \
		mv metalock_theme-${theme_name}-${theme_version}.tar.bz2 ../dist/

install: all
	@echo installing metalock
.if !exists(../themes/${DEFAULT_THEME}/metadata.mk)
	@echo -n "error: missing ../themes/${DEFAULT_THEME}/metadata.mk "
	@echo "(the default theme is missing)"
.else
	@echo installing executable file to ${PREFIX}/bin
	@mkdir -p ${PREFIX}/bin
	@cp -f metalock ${PREFIX}/bin
	@chmod 755 ${PREFIX}/bin/metalock
	@chmod u+s ${PREFIX}/bin/metalock
	@echo -n "installing theme to "
	@echo "${PREFIX}/share/metalock/themes/${DEFAULT_THEME}"
	@mkdir -p ${PREFIX}/share/metalock/themes
	@cp -pRf ../themes/${DEFAULT_THEME} ${PREFIX}/share/metalock/themes
	@chown 0:0 ${PREFIX}/share/metalock/themes/${DEFAULT_THEME}/*
	@chmod 644 ${PREFIX}/share/metalock/themes/${DEFAULT_THEME}/*
	@echo "finished installing metalock"
.endif

uninstall:
	@echo removing executable file from ${PREFIX}/bin
	@rm -rf ${PREFIX}/share/metalock/themes/${DEFAULT_THEME}
	@rmdir ${PREFIX}/share/metalock/themes
	@rmdir ${PREFIX}/share/metalock
	@rm -f ${PREFIX}/bin/metalock

.PHONY: all options clean dist install uninstall
