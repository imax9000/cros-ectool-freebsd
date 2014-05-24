--- ./util/comm-i2c.c.orig	2013-03-12 05:08:56.000000000 +0000
+++ ./util/comm-i2c.c	2014-05-24 16:19:03.000000000 +0100
@@ -7,8 +7,6 @@
 
 #include <errno.h>
 #include <fcntl.h>
-#include <linux/i2c.h>
-#include <linux/i2c-dev.h>
 #include <stdint.h>
 #include <stdio.h>
 #include <stdlib.h>
@@ -16,6 +14,7 @@
 #include <sys/ioctl.h>
 #include <sys/stat.h>
 #include <sys/types.h>
+#include <dev/iicbus/iic.h>
 
 #include "comm-host.h"
 
@@ -24,7 +23,7 @@
 #define I2C_ADAPTER_NODE "/sys/class/i2c-adapter/i2c-%d/name"
 #define I2C_ADAPTER_NAME "cros_ec_i2c"
 #define I2C_MAX_ADAPTER  32
-#define I2C_NODE "/dev/i2c-%d"
+#define I2C_NODE "/dev/iic%d"
 
 #ifdef DEBUG
 #define debug(format, arg...) printf(format, ##arg)
@@ -36,38 +35,15 @@
  *   OUT: (version, command, size, ... request ..., checksum) */
 #define PROTO_V2_IN    4
 /*   IN:  (command, size, ... response ..., checkcum) */
-#define PROTO_V2_OUT   3
+#define PROTO_V2_OUT   4
 
 static int i2c_fd = -1;
 
 int comm_init(void)
 {
 	char *file_path;
-	char buffer[64];
-	FILE *f;
-	int i;
 
-	/* find the device number based on the adapter name */
-	for (i = 0; i < I2C_MAX_ADAPTER; i++) {
-		if (asprintf(&file_path, I2C_ADAPTER_NODE, i) < 0)
-			return -1;
-		f = fopen(file_path, "r");
-		if (f) {
-			if (fgets(buffer, sizeof(buffer), f) &&
-			    !strncmp(buffer, I2C_ADAPTER_NAME, 6)) {
-				free(file_path);
-				break;
-			}
-			fclose(f);
-		}
-		free(file_path);
-	}
-	if (i == I2C_MAX_ADAPTER) {
-		fprintf(stderr, "Cannot find I2C adapter\n");
-		return -1;
-	}
-
-	if (asprintf(&file_path, I2C_NODE, i) < 0)
+	if (asprintf(&file_path, I2C_NODE, 0) < 0)
 		return -1;
 	debug("using I2C adapter %s\n", file_path);
 	i2c_fd = open(file_path, O_RDWR);
@@ -89,7 +65,7 @@
 int ec_command(int command, int version, const void *indata, int insize,
 	       void *outdata, int outsize)
 {
-	struct i2c_rdwr_ioctl_data data;
+	struct iic_rdwr_data data;
 	int ret = -1;
 	int i;
 	int req_len;
@@ -99,7 +75,7 @@
 	const uint8_t *c;
 	uint8_t *d;
 	uint8_t sum;
-	struct i2c_msg i2c_msg[2];
+	struct iic_msg i2c_msg[2];
 
 	if (version > 1) {
 		fprintf(stderr, "Command versions >1 unsupported.\n");
@@ -111,15 +87,10 @@
 		return -EC_RES_ERROR;
 	}
 
-	if (ioctl(i2c_fd, I2C_SLAVE, EC_I2C_ADDR) < 0) {
-		fprintf(stderr, "Cannot set I2C slave address\n");
-		return -EC_RES_ERROR;
-	}
-
-	i2c_msg[0].addr = EC_I2C_ADDR;
-	i2c_msg[0].flags = 0;
-	i2c_msg[1].addr = EC_I2C_ADDR;
-	i2c_msg[1].flags = I2C_M_RD;
+	i2c_msg[0].slave = EC_I2C_ADDR;
+	i2c_msg[0].flags = IIC_M_WR;
+	i2c_msg[1].slave = EC_I2C_ADDR;
+	i2c_msg[1].flags = IIC_M_RD;
 	data.msgs = i2c_msg;
 	data.nmsgs = 2;
 
@@ -160,19 +131,21 @@
 	i2c_msg[1].buf = (char *)resp_buf;
 
 	/* send command to EC and read answer */
-	ret = ioctl(i2c_fd, I2C_RDWR, &data);
+  do {
+	ret = ioctl(i2c_fd, I2CRDWR, &data);
 	if (ret < 0) {
 		fprintf(stderr, "i2c transfer failed: %d (err: %d)\n",
 			ret, errno);
 		ret = -EC_RES_ERROR;
 		goto done;
 	}
+  } while (ret < 0 && errno == EINTR);
 
 	/* check response error code */
-	ret = resp_buf[0];
+	ret = resp_buf[1];
 	/* TODO: handle EC_RES_IN_PROGRESS case. */
 
-	resp_len = resp_buf[1];
+	resp_len = resp_buf[2];
 	if (resp_len > outsize) {
 		fprintf(stderr, "response size is too large %d > %d\n",
 				resp_len, outsize);
@@ -182,21 +155,26 @@
 
 	if (ret) {
 		debug("command 0x%02x returned an error %d\n",
-			 command, i2c_msg[1].buf[0]);
+			 command, i2c_msg[1].buf[1]);
+    debug("full response: ");
+    for (i = 0; i < i2c_msg[1].len; i++) {
+      debug("%02x ", i2c_msg[1].buf[i]);
+    }
+    debug("\n");
 		/* Translate ERROR to -ERROR */
 		ret = -ret;
 	} else if (outsize) {
 		debug("i2c resp  :");
 		/* copy response packet payload and compute checksum */
-		sum = resp_buf[0] + resp_buf[1];
+		sum = resp_buf[1] + resp_buf[2];
 		for (i = 0, d = outdata; i < resp_len; i++, d++) {
-			*d = resp_buf[i + 2];
+			*d = resp_buf[i + 3];
 			sum += *d;
 			debug(" %02x", *d);
 		}
 		debug(", sum=%02x\n", sum);
 
-		if (sum != resp_buf[resp_len + 2]) {
+		if (sum != resp_buf[resp_len + 3]) {
 			fprintf(stderr, "bad packet checksum\n");
 			ret = -EC_RES_ERROR;
 			goto done;
