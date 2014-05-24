--- ./util/ectool_keyscan.c.orig	2013-03-12 05:08:56.000000000 +0000
+++ ./util/ectool_keyscan.c	2014-05-24 16:19:03.000000000 +0100
@@ -9,6 +9,7 @@
 #include <string.h>
 #include <termios.h>
 #include <unistd.h>
+#include <sys/endian.h>
 #include <sys/stat.h>
 
 #include "comm-host.h"
