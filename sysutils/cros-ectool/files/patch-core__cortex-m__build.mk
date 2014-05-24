--- ./core/cortex-m/build.mk.orig	2013-03-12 05:08:56.000000000 +0000
+++ ./core/cortex-m/build.mk	2014-05-24 16:19:03.000000000 +0100
@@ -10,7 +10,7 @@
 CFLAGS_FPU-$(CONFIG_FPU)=-mfpu=fpv4-sp-d16 -mfloat-abi=hard
 
 # CPU specific compilation flags
-CFLAGS_CPU=-mcpu=cortex-m4 -mthumb -Os -mno-sched-prolog
+CFLAGS_CPU=-march=armv6 -Os
 CFLAGS_CPU+=-mno-unaligned-access
 CFLAGS_CPU+=$(CFLAGS_FPU-y)
 
