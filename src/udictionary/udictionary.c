#include "ustandard/ustandard_sys.h"
#include "ustandard/utypes.h"













typedef struct udictionary* udictionary_t;
udictionary_t udictionary_create(uf_pointer_free key_free, uf_pointer_free value_free);
void udictionary_destroy(udictionary_t dict);

int udictionary_set(udictionary_t dict, void* key, void* value);
void* udictionary_get(udictionary_t dict, void* key);
int udictionary_remove(udictionary_t dict, void* key);
int udictionary_clear(udictionary_t dict);
