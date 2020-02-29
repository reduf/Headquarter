# Headquarter

CFLAGS	?= -O2 -pedantic
LDFLAGS ?= -pthread -ldl -lm -export-dynamic
WARNING_CFLAGS ?= \
	-Wno-missing-field-initializers \
	-Wno-int-to-pointer-cast \
	-Wno-visibility

DFLAGS ?= -D_POSIX_C_SOURCE=200809L

.SILENT:
.PHONY: clean

LOCAL_CFLAGS = $(WARNING_CFLAGS) \
	-Imbedtls-2.4.0/include -Iinclude -Icode -Iinclude/linux

LOCAL_LDFLAGS =

client: code/client/main.c
	$(MAKE) -C mbedtls-2.4.0 lib
	$(CC) $(LOCAL_CFLAGS) code/client/main.c \
		-lmbedtls-2.4.0/library/ibmbedcrypto.a -std=c11 $(DFLAGS) $(LDFLAGS)

.c.o:
	echo "  CC    $<"
	$(CC) $(LOCAL_CFLAGS) $(CFLAGS) -c $<

clean:
	$(MAKE) -C mbedtls-2.4.0 clean
