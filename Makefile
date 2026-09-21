
lfcaps: lfcaps.h lfcaps.c cap_table.h
	gcc -Os -o lfcaps lfcaps.c -DLFCAPS_STANDALONE

