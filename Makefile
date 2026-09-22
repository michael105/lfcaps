
lfcaps: lfcapslib.h lfcapslib.c lfcaps.c
	gcc -Os -o lfcaps lfcaps.c lfcapslib.c -DLFCAPS_STANDALONE

