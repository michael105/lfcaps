



typedef struct _capname_s { uint cap; char *name; char* shortname; } capname_s;

#define CN( cap, name, shortname ) { cap, name, shortname },
capname_s capnames[] = {
#include "cap_table.h"
};
#undef CN

#define CAPCOUNT (sizeof( capnames ) / sizeof( capname_s ))

#define CN( cap, name, shortname ) sizeof(name)+
const int maxcaplen = 
#include "cap_table.h"
 + CAPCOUNT*5; // currently 626 bytes
#undef CN

#define MAX_CAPLEN (maxcaplen)


