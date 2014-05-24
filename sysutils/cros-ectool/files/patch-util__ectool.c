--- ./util/ectool.c.orig	2013-03-12 05:08:56.000000000 +0000
+++ ./util/ectool.c	2014-05-24 16:19:03.000000000 +0100
@@ -9,7 +9,7 @@
 #include <stdio.h>
 #include <stdlib.h>
 #include <string.h>
-#include <sys/io.h>
+//#include <sys/io.h>
 #include <unistd.h>
 
 #include "battery.h"
@@ -586,7 +586,7 @@
 		return -1;
 	}
 	p.offset = strtol(argv[1], &e, 0);
-	if ((e && *e) || p.offset < 0 || p.offset > 0x100000) {
+	if ((e && *e) || p.offset > 0x100000) {
 		fprintf(stderr, "Bad offset.\n");
 		return -1;
 	}
@@ -681,17 +681,17 @@
 
 int cmd_serial_test(int argc, char *argv[])
 {
-	const char *c = "COM2 sample serial output from host!\r\n";
+//  const char *c = "COM2 sample serial output from host!\r\n";
 
 	printf("Writing sample serial output to COM2\n");
 
-	while (*c) {
+//	while (*c) {
 		/* Wait for space in transmit FIFO */
-		while (!(inb(0x2fd) & 0x20)) {}
+//		while (!(inb(0x2fd) & 0x20)) {}
 
 		/* Put the next character */
-		outb(*c++, 0x2f8);
-	}
+//		outb(*c++, 0x2f8);
+//	}
 
 	printf("done.\n");
 	return 0;
@@ -1045,7 +1045,7 @@
 
 #define LBMSG(state) #state
 #include "lightbar_msg_list.h"
-static const char const *lightbar_cmds[] = {
+static char const *lightbar_cmds[] = {
 	LIGHTBAR_MSG_LIST
 };
 #undef LBMSG
@@ -2496,10 +2496,10 @@
 /* Flood port 80 with byte writes */
 int cmd_port_80_flood(int argc, char *argv[])
 {
-	int i;
+//	int i;
 
-	for (i = 0; i < 256; i++)
-		outb(i, 0x80);
+//	for (i = 0; i < 256; i++)
+//		outb(i, 0x80);
 	return 0;
 }
 
