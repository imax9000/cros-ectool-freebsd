--- ./util/lock/csem.c.orig	2013-03-12 05:08:56.000000000 +0000
+++ ./util/lock/csem.c	2014-05-24 16:19:03.000000000 +0100
@@ -45,7 +45,7 @@
 
 #include "csem.h"
 
-#if defined(__GNU_LIBRARY__) && !defined(_SEM_SEMUN_UNDEFINED)
+#if defined(__FreeBSD__) || defined(__GNU_LIBRARY__) && !defined(_SEM_SEMUN_UNDEFINED)
 /* union semun is defined by including <sys/sem.h> */
 #else
 /* according to X/OPEN we have to define it ourselves */
@@ -254,7 +254,7 @@
 	sops.sem_num = 0;
 	sops.sem_op = -1;
 	sops.sem_flg = undoflag;
-	return semtimedop(sem_id, &sops, 1, timeout);
+	return semop(sem_id, &sops, 1);
 }
 
 int csem_down_timeout(int sem_id, struct timespec *timeout)
