#ifndef __UCONF_PARSE_H__
#define __UCONF_PARSE_H__
#include <stdio.h>
__BEGIN_DECLS

typedef struct conf CONF;
typedef enum{

    conf_parse_e            = 0,
    conf_update_e, 
    conf_crate_e,

}conf_mode_e;


CONF* uconf_open(const char* path, conf_mode_e mode);

int uconf_get_category_num(CONF* conf);
const char* uconf_get_category(CONF* conf, int index_category);

const char* uconf_get_string(CONF* conf, const char* name_category, const char* name_item);

int uconf_close(CONF* conf);


__END_DECLS
#endif /* uconf-parse.h */
