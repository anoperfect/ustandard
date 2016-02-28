#include "ustandard/ustandard.h"




#define test_writevs ufile_writevs

int main()
{
while(1){
    CONF* conf;
    const char* path;
    path = "input/1.conf";
    conf = uconf_open(path, conf_parse_e);
    if(NULL == conf) {
        exit(EXIT_FAILURE);
    }

    const char* path_export = "export/export";
    test_writevs(path_export, "w", "");
    int num_category = uconf_get_category_num(conf);
    const char* name_category;
    //const char* name_item;
    const char* value;
    int i;
    for(i=0; i<num_category; i++)
    {
        name_category = uconf_get_category(conf, i);
        test_writevs(path_export, "a", "%s\n", name_category);
        value = uconf_get_string(conf, name_category, "xxxxxx");
        test_writevs(path_export, "a", "\t%s------%s\n", "xxxxxx", value);
        value = uconf_get_string(conf, name_category, "123456");
        test_writevs(path_export, "a", "\t%s------%s\n", "123456", value);
    }
    
    uconf_close(conf);

break; }

    return 0;
}
