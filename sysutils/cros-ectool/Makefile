# $FreeBSD$

PORTNAME=	cros-ectool
PORTVERSION=	20130312
CATEGORIES=	sysutils
MASTER_SITES=	https://github.com/gelraen/cros-ec/archive/${COMMIT}.zip?dummy=/

MAINTAINER=	gelraen.ua@gmail.com
COMMENT=	ChromeOS Embedded Controller CLI

LICENSE=	BSD3CLAUSE
LICENSE_FILE=	${WRKSRC}/LICENSE

ONLY_FOR_ARCHS=	armv6

COMMIT=	f5f30a22c676f1450052cd7d9451ff650818ba3f

OPTIONS_DEFINE=	DEBUG
DEBUG_DESC=	Enable debug output

OPTIONS_SINGLE=	MODEL
OPTIONS_SINGLE_MODEL=	SNOW SPRING
SNOW_DESC=	Samsung Chromebook
SPRING_DESC=	HP Chromebook 11

USES+=	gmake

PLIST_FILES=	sbin/cros-ectool

WRKSRC=	${WRKDIR}/cros-ec-${COMMIT}

.include <bsd.port.options.mk>

.if ${PORT_OPTIONS:MSNOW}
BOARD=	snow
.elif ${PORT_OPTIONS:MSPRING}
BOARD=	spring
.endif

.if ${PORT_OPTIONS:MDEBUG}
GMAKE_ARGS+=	EXTRA_CFLAGS=-DDEBUG
.endif

GMAKE_ARGS+=	BOARD=${BOARD}
GMAKE_ARGS+=	CROSS_COMPILE= HOST_CROSS_COMPILE=

GMAKE_ARGS+=	build/${BOARD}/util/ectool

do-build:
	cd ${WRKSRC} && ${GMAKE} ${GMAKE_ARGS}

do-install:
	${INSTALL_PROGRAM} ${WRKSRC}/build/${BOARD}/util/ectool ${STAGEDIR}${PREFIX}/sbin/cros-ectool

.include <bsd.port.mk>
