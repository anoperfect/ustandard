#define LEN_LINE                    1024
#define MAX_CATEGORY                100
#define MAX_ITEM_IN_CATEGORY        100


char kchars_blank[] = {'\r', '\n', ' ', '\t'};

struct conf_item
{
    char* name;
    char* value;
};


struct conf_category
{
    char* name;
    int num_conf_items;
    struct conf_item items[MAX_ITEM_IN_CATEGORY];
};


struct conf
{
    char* content;
    int num_conf_category;
    struct conf_category category[MAX_CATEGORY];
};
typedef struct conf CONF;


/*
    0. not recognized.
    1. category.
    2. item.
    3. comment.
    -1 : ENOMEM.
*/
int _uconf_parse_line(char* line, struct conf_item* item, char** category);
int _uconf_add_category(CONF* conf, char* name_category);
int _uconf_add_item(CONF* conf, struct conf_item* item);
int _uconf_parse(CONF* conf);


char* __ustrndup(const char* str, size_t n)
{
    char* dest = um_malloc(n + 1);
    if(NULL != dest) {
        memcpy(dest, str, n);
        dest[n] = '\0';
    }

    return dest;
}


int __ustrtrim_char_include(char ch, char s[], size_t n)
{
    int ret = 0;
    int i;
    for(i=0; i<n; i++) {
        if(ch == s[i])
            return 1;
    }

    return ret;
}


char* __ustrtrim_chars(char* str, char s[], size_t n)
{
    int len_trim_head = 0;
    int len_trim_tail = 0;
    int len_str = 0;

    len_str = strlen(str);
    if(len_str == 0) {
        return str;
    }

    char* tmp = str;
    while(*tmp != '\0' && __ustrtrim_char_include(*tmp, s, n))
    {
        len_trim_head ++ ;
        tmp ++ ;
    }

    if(len_trim_head == len_str) {
        str[0] = '\0';
        return str;
    }

    tmp = str + (len_str-1);
    while(__ustrtrim_char_include(*tmp, s, n)) {
        tmp -- ;
        len_trim_tail ++ ;
    }

    len_str -= (len_trim_head + len_trim_tail);
    if(len_trim_head != 0) {
        int i;
        for(i=0; i<len_str; i++)
        {
            str[i] = str[i+len_trim_head];
        }
    }
    
    str[len_str] = '\0';
    
    return str;
}


CONF* ulogconf_open(const char* path)
{
    int ret =0;
    int size_file = _ufile_length(path);
    if(size_file == -1 || size_file < 7) {
        __error_printf("%s : conf file error.\n", "conf");
        return NULL;
    }

    char* content = um_malloc(size_file + 1);
    if(NULL == content) {
        return NULL;
    }

    content[size_file] = '\0';
    size_t ncopy = _ufile_copy_to_buffer(path, content, size_file);
    if(ncopy != size_file) {
        __error_printf("%s : read conf file error.\n", "conf");
        um_free(content);
        content = NULL;
        return NULL;
    }

    CONF* conf = um_malloc(sizeof(CONF));
    if(NULL == conf) {
        um_free(content);
        content = NULL;
        return NULL;
    }
    memset(conf, 0, sizeof(CONF));
    conf->content = content;

    ret = _uconf_parse(conf);

    return conf;
}


int _uconf_parse_line(char* line, struct conf_item* item, char** category)
{
    int ret = 0; 
    size_t len;
    char* tmp;

    __ustrtrim_chars(line, kchars_blank, sizeof(kchars_blank));
    len = strlen(line);

    if(len >3 && line[0] == '[' && line[len-1] == ']') {
        ret = 1;
        int len_category = len -2 ;
        *category = __ustrndup(line+1, len_category);
        if(NULL == *category) {
            ret = -1;
            goto finish;
        }
        __ustrtrim_chars(*category, kchars_blank, sizeof(kchars_blank));
    }
    else if(len > 3 && NULL != (tmp = strchr(line, '=')) && tmp > line && tmp < line+(len-1)) {
        ret = 2;
        int len_name = tmp - line;
        int len_value = line + (len-1) - tmp;
        item->name = __ustrndup(line, len_name);
        if(NULL == item->name) {
            ret = -1;
            goto finish;
        }
        __ustrtrim_chars(item->name, kchars_blank, sizeof(kchars_blank));

        item->value = __ustrndup(tmp+1, len_value);
        if(NULL == item->value) {
            um_free(item->name);
            item->name = NULL;
            ret = -1;
            goto finish;
        }
        __ustrtrim_chars(item->value, kchars_blank, sizeof(kchars_blank));
    }
    else if(line[0] == '#') {
        ret = 3;
    }

finish:
    return ret;
}


int _uconf_add_category(CONF* conf, char* name_category)
{
    int ret = 0;

    if(conf->num_conf_category < MAX_CATEGORY) {
        conf->category[conf->num_conf_category].name = name_category;
        conf->num_conf_category ++ ;
    }
    else {
        __error_printf("%s : category full.\n", "conf");
        ret = -1;
    }
    
    return ret;
}


int _uconf_add_item(CONF* conf, struct conf_item* item)
{
    int ret = 0;

    if(conf->num_conf_category == 0) {
        conf->category[0].name = __ustrndup("*", 1);
        if(NULL == conf->category[0].name) {
            ret = -1;
            return ret;
        }
        conf->num_conf_category ++ ;
    }

    struct conf_category* category = NULL;
    category = &conf->category[conf->num_conf_category-1];
    
    if(category->num_conf_items < MAX_ITEM_IN_CATEGORY) {
        category->items[category->num_conf_items] = *item;
        category->num_conf_items ++ ;
    }
    else {
        __error_printf("%s : category item full.\n", "conf");
        ret = -1;
    }

    return ret;
}


int _uconf_parse(CONF* conf)
{
    int ret = 0;

    char line[LEN_LINE];
    memset(line, 0, LEN_LINE);

    int len_read = 0;
    char* tmp = conf->content;
    
    while(*tmp != '\0')
    {
        /* read line. */
        len_read = 0;
        while(*tmp != '\r' && *tmp != '\n' && len_read < LEN_LINE-1)
        {
            len_read ++ ;
            tmp ++ ;
        }
        memcpy(line, tmp-len_read, len_read);
        line[len_read] = '\0';

        /* parse line. */
        struct conf_item tmp_item;
        char* name_category = NULL;
        int ret_parse = _uconf_parse_line(line, &tmp_item, &name_category); 
        if(1 == ret_parse) {
            _uconf_add_category(conf, name_category);
        }
        else if(2 == ret_parse) {
            _uconf_add_item(conf, &tmp_item);
        }

        /* skip tailer \r\n. */
        while(*tmp != '\0' && (*tmp == '\r' || *tmp == '\n')) {
            tmp ++ ;
        }
    }

    return ret;
}


int ulogconf_get_category_num(CONF* conf)
{
    int num = conf->num_conf_category;
    return num;
}


const char* ulogconf_get_category(CONF* conf, int index_category)
{
    const char* name;
    if(index_category >= 0 && index_category < conf->num_conf_category) {
        name = conf->category[index_category].name;
    }
    else {
        name = NULL;
    }

    return name;
}


/*
    Input name_category to NULL or "*" means wildcard any category.
*/
const char* ulogconf_get_string(CONF* conf, const char* name_category, const char* name_item)
{
    const char* value = NULL;

    int i, j;
    struct conf_category* category = NULL;
    struct conf_item* item;

    int category_wildcard = 0;
    if(NULL == name_category || 0 == strcmp(name_category, "*")) {
        category_wildcard = 1;
    }

    for(i=0; i<conf->num_conf_category && NULL == value; i++)
    {
        category = &conf->category[i];
        if(0 == category_wildcard 
                && (NULL == category->name || 0 != strcmp(category->name, name_category))) {
            continue;
        }

        for(j=0; j<category->num_conf_items; j++)
        {
            item = &category->items[j];
            if(NULL != item->name && 0 == strcmp(item->name, name_item)) {
                value = item->value;
                break;
            }
        }
    }

    return value;
}


struct conf_category* _ulogconf_get_category_by_name(CONF* conf, const char* name_category)
{
    int i;
    for(i=0; i<conf->num_conf_category; i++)
    {
        struct conf_category* category = &conf->category[i];
        if((NULL != category->name && 0 == strcmp(category->name, name_category))) {
            return category;
        }
    }

    return NULL;
}


int ulogconf_get_item_num(CONF* conf, const char* name_category)
{
    int num = 0;

    struct conf_category* category = _ulogconf_get_category_by_name(conf, name_category);
    if(NULL != category) {
        num = category->num_conf_items;
    }

    return num;
}


int ulogconf_get_item_by_index(CONF* conf, const char* name_category, int idx, 
        char* name, size_t size_name, 
        char* value, size_t size_value)
{
    int ret = 0;
    
    struct conf_category* category = _ulogconf_get_category_by_name(conf, name_category);
    if(NULL == category) {
        __error_printf("%s : category not exist.\n", __FUNCTION__);
        ret = -1;
        return ret;
    }

    int num = category->num_conf_items;
    if(idx >= num) {
        __error_printf("%s : idx>num.\n", __FUNCTION__);
        ret = -1;
        return ret;
    }

    __strcpy(name, size_name, category->items[idx].name);
    __strcpy(value, size_value, category->items[idx].value);

    return ret;
}


int ulogconf_close(CONF* conf)
{
    int ret = 0;

    int i, j;
    struct conf_category* category = NULL;
    struct conf_item* item;
    for(i=0; i<conf->num_conf_category; i++)
    {
        category = &conf->category[i];
        for(j=0; j<category->num_conf_items; j++)
        {
            item = &category->items[j];
            if(NULL != item->name) {
                um_free(item->name);
                item->name = NULL;
            }

            if(NULL != item->value) {
                um_free(item->value);
                item->value = NULL;
            }
        }

        if(NULL != category->name) {
            um_free(category->name);
            category->name = NULL;
        }
    }

    um_free(conf->content);
    um_free(conf);
    conf = NULL;

    return ret;
}
