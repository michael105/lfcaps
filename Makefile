
lfcaps: lfcapslib.h lfcapslib.c lfcaps.c
	gcc -o lfcaps lfcaps.c lfcapslib.c -DLFCAPS_STANDALONE

