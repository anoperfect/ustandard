#include "ustandard/ustandard.h"




char* kfiles_not_monitor[] = {
    "ngx_string.c", 
    NULL
};


char* kfunctions_not_monitor[] = {
    "ngx_vslprintf", 
    "ngx_sprintf", 
    "ngx_sprintf_num", 
    "ngx_strlow", 
    "ngx_alloc", 
    "ngx_palloc", 
    "ngx_pnalloc", 
    "ngx_palloc_small", 
    "ngx_pcalloc", 
    "ngx_palloc_large", 
    "ngx_calloc", 
    "ngx_pnalloc", 
    "ngx_array_push", 
    "ngx_array_push_n", 
    "ngx_hash_add_key", 
    "ngx_hash_key", 
    "ngx_hash_key_lc", 
    "ngx_hash_strlow", 
    "", 
    "", 
    "", 
    "", 
    NULL
};


char* kfunctions_not_detail[] = {
    "ngx_vslprintf", 
    NULL
};


struct parsec_statics {
    int number_function;
    int number_return_function;
};


struct parsec_statics kstatics = {0};



struct parseline {
    long offset;
    char* c;
    size_t len;
    
    int type;
    int start;
    int end;
};


struct parsef {
    const char* filename;
    char* content;
    size_t len;

    char* parse;

    struct parseline* lines;
    long nlines;
    long nlines_total;

    int number_function;


    struct ustr_replace_member replace_members[1024];
    int nreplace_members ;

    void* freeptr[1024];
    int nfreeptr;
};


struct function_property {
    char type[1024];
    char name[1024];
};


/* clear previous added. */
int clear_enter_return(const char* filename)
{
    int ret = 0;

    ufile_replaces(filename, filename, "ENTER__", "");
    ufile_replaces(filename, filename, "RETURN_VOID_ADD__", "");
    ufile_replaces(filename, filename, "RETURN_END__", "");
    ufile_replaces(filename, filename, "RETURN__", "return");

    size_t size = 0;
    char* parse = ufile_dup(filename, &size);

    struct urange ranges[1024];
    long n = ustr_ranges(parse, 
        "RETURN_TYPE__", ",",
        ranges, 1024);
    n = n;

    if(n > 0) {
        ustr_t str;
        ret =  ustr_replace_ranges(parse, 
            "return", 
            n, 
            ranges, 
    	    &str);

        um_free(parse);
        parse = str.s;
    }

    ustr_replaces_reuse(parse, strlen(parse)+1, " RETURN_TYPE_END__)", "");

    ufile_write(filename, "w", parse, strlen(parse));
    um_free(parse);

    return ret;
}


/*
 */
struct parsef* read_file(const char* filename)
{
    uloginf("action %s\n", "read_file");
    struct parsef* f = NULL;
    f = um_malloc(sizeof(*f));
    memset(f, 0, sizeof(*f));

    f->filename = filename;
    f->content = ufile_dup(f->filename, &f->len);
    assert(f->content);
    assert(f->len > 0);

    f->parse = ustrdup(f->content);

    return f;
}


int parse_free(struct parsef* f)
{
    int ret = 0;

    um_free(f->content);
    um_free(f->parse);

    long idx;
    for(idx=0; idx<f->nlines; idx++) {
        um_free(f->lines[idx].c);
    }

    for(idx=0; idx<f->nfreeptr; idx++) {
        um_free(f->freeptr[idx]);
    }
    um_free(f->lines);
    
    um_free(f);

    char s[1024*100];
    umalloc_query(s, sizeof(s));

    uloginf("------\n%s\n\n", s);

    return ret;
}


int addto_delayfree(struct parsef* f, void* ptr)
{
    int ret = 0;

    assert(f->nfreeptr < 1024);
    f->freeptr[f->nfreeptr] = ptr;
    f->nfreeptr ++;

    return ret;
}


int parse_file_line(struct parsef* f)
{
    int ret = 0;

    uloginf("action %s\n", "parse_file_line");

    f->nlines_total = 102400;
    f->lines = um_malloc(sizeof(struct parseline) * f->nlines_total);
    f->nlines = 0;

    char* start = f->content;
    char* t = start;

    while(1) {
        char* tmp = strchr(t, '\n');
        if(!tmp) {
            break;
        }

        struct parseline *line = &f->lines[f->nlines];
        f->nlines ++; 
        assert(f->nlines < f->nlines_total);
        line->offset = t - start;
        line->len = tmp - t + 1;
        line->c = ustrndup(t, line->len);
        //uloginf("line number : %ld. offset%ld, len%zd\n", f->nlines, line->offset, line->len);

        t = tmp + 1;
    }

    if(*t != '\0') {
        struct parseline *line = &f->lines[f->nlines];
        f->nlines ++;
        assert(f->nlines < f->nlines_total);
        line->offset = t - start;
        line->len = strlen(t); 
        line->c = ustrndup(t, line->len);
    }

    uloginf("line number : %ld\n", f->nlines);

    return ret;
}


int count_line(struct parsef* f, long offset)
{
    bool found = false;
    int idx;
    for(idx=0; idx<f->nlines; idx++) {
        if(offset >= f->lines[idx].offset && offset < (f->lines[idx].offset + f->lines[idx].len)) {
            found = true;
            break;
        }
    }

    if(!found) {
        ulogerr("offset %ld\n", offset);
    }

    assert(found);
    return idx+1;
}


const char* kneed_clear[] = {
    "ngx_cdecl", 
    "ngx_libc_cdecl", 
    NULL
};


int remove_need_clear(struct parsef* f)
{
    int ret = 0;

    const char** pps = kneed_clear;
    while(*pps) {
        char to[1024];
        size_t len_s = strlen(*pps);
        assert(len_s < sizeof(to));
        int idx;
        for(idx=0; idx<len_s; idx++) {
            to[idx] = ' ';
        }
        to[len_s] = '\0';

        ustr_replaces_reuse(f->parse, strlen(f->parse)+1, *pps, to);

        pps ++;
    }

    return ret;
}


int remove_comments(struct parsef* f)
{
    int ret = 0;

    uloginf("action %s\n", "remove_comments");

    struct urange ranges_comment[1024];
    long n = ustr_ranges(f->parse, "/*", "*/", ranges_comment, 1024);
    uloginf("n=%ld\n", n);
    long idx = 0;
    for(idx=0; idx<n; idx++) {
        long charidx ;
        uloginf("clear comment range : [%ld,%ld]\n", ranges_comment[idx].location, ranges_comment[idx].length);
        for(charidx=ranges_comment[idx].location; charidx<ranges_comment[idx].location + ranges_comment[idx].length; charidx++) {

            //uloginf("charidx=%ld\n", charidx);

            if(f->parse[charidx] != '\n' && f->parse[charidx] != '\r') {
                f->parse[charidx] = ' ';
            }

            //uloginf("charidx=%ld\n", charidx);
        }
        //memset(f->parse+ranges_comment[idx].location, ' ', ranges_comment[idx].length);
    }
    //ufile_write("0comment.cpp", "w", f->parse, strlen(f->parse));

    uloginf("finish %s\n", "remove_comments");
    return ret;
}


int remove_cstrings(struct parsef* f)
{
    int ret = 0;

    uloginf("action %s\n", "remove_cstrings");

    /* clear '\"' && '"' . */
    //ustr_t str;
    ustr_replaces_reuse(f->parse, strlen(f->parse)+1, "'\\\"'", "    "); 
    ustr_replaces_reuse(f->parse, strlen(f->parse)+1, "'\"'", "   ");
    ustr_replaces_reuse(f->parse, strlen(f->parse)+1, "'}'", "'|'");
    ustr_replaces_reuse(f->parse, strlen(f->parse)+1, "'{'", "'|'");
    ustr_replaces_reuse(f->parse, strlen(f->parse)+1, "';'", "' '");

    //ufile_write("01cstring.cpp", "w", f->parse, strlen(f->parse));

    char* t = f->parse;
    while(1) {
        char* s = strchr(t, '"');
        if(!s) {
            break;
        }

        if(s>f->parse && (*(s-1) == '\\' )) {
            t = s+1;
            continue;
        }

        t = s+1;
        char* e = NULL;
        while(1) {
            e = strchr(t, '"');
            assert(e);

            if(*(e-1) == '\\' ) {
                t = e+1;
                continue;
            }

            break;
        }

        int line_s = count_line(f, s-f->parse);
        int line_e = count_line(f, e-f->parse);
        if(line_s != line_e) {
            ulogerr("%d %d\n", line_s, line_e);
        }
        assert(line_s == line_e);

        memset(s+1, ' ', e - s - 1);

        t = e + 1;
    }

    //ufile_write("0cstrings.cpp", "w", f->parse, strlen(f->parse));

    return ret;
}

int connect_cr(struct parsef* f)
{
    int ret = 0;

    ustr_replaces_reuse(f->parse, strlen(f->parse)+1, 
        "\\\n", "  ");
    
    return ret;
}


int remove_define(struct parsef* f)
{
    uloginf("action %s\n", "remove_define");
    int ret = 0;

    char *t = f->parse;
    while(1) {
        char* s = strchr(t, '#');
        if(!s) {
            break;
        }

        if(s == f->parse || *(s-1) == '\n') {
            char* cr = strchr(s, '\n');
            if(cr) {
                memset(s, ' ', cr-s);
                t = cr + 1;
            }
            else {
                memset(s, ' ', strlen(s));
                break;
            }
        }
        else {
            t =  s + 1;
        }
    }
    //ufile_write("0define.cpp", "w", f->parse, strlen(f->parse));

    return ret;
}


bool function_not_monitor(const char* function)
{
    bool ret = false;

    char** f = kfunctions_not_monitor;
    while(*f) {
        if(0 == strcmp(function, *f)) {
            return true;
        }

        f ++;
    }

    return ret;
}


bool function_not_detail(const char* function)
{
    bool ret = false;
    return ret;
}


char* skip_blank(char* t)
{
    char* s = t;
    while(*s == ' ' || *s == '\t' || *s == '\n') {
        s ++;
    }

    return s;
}


char* find_pair(struct parsef* f, char* s)
{
    int level = 0;
    char* t = s;

    while(t && *t) {
        if(*t == '}') {
            assert(level >= 0);
            if(level == 0) {
                return t;
            }
            else {
                ulogdbg("level%d }   --- line%d\n", level, count_line(f, t-f->parse));
                level --;
            }

        }
        else if(*t == '{') {
            level ++;
            ulogdbg("level%d {   --- line%d\n", level, count_line(f, t-f->parse));
        }
        else {

        }

        t ++;
    }

    return NULL;
}


void printl(struct parsef* f, char* start, char* end, const char* name)
{
    if(0 != strcmp(name, "{}block")) return;



    char* s0 = f->parse;
    int line_start = count_line(f, start - s0);
    int line_end = count_line(f, end - s0);

    if(line_start == line_end) {
        uloginf("%11d : %s.\n", line_start, name);
    }
    else {
        uloginf("%4d - %4d : %s cross lines.\n", line_start, line_end, name);
    }
}


int replace_range_add(struct parsef* f, struct urange range, const char* to, bool monitor)
{
    int ret = 0;
    if(!monitor) {
        return ret;
    }

    struct ustr_replace_member* r = &f->replace_members[f->nreplace_members];
    f->nreplace_members ++;
    assert(f->nreplace_members < 1024);

    r->range.location = range.location;
    r->range.length = range.length;
    r->to = to;
    r->to_size = strlen(r->to);

    return ret;
}


int function_define_parse(struct parsef* f, char* start, char* left, char* rigth, 
        struct function_property* funcp 
        )
{
    int ret = 0;

    char sh[1024];
    ustrncpy(sh, sizeof(sh), start, left - start);

    ustrtrim_chars(sh, true, true, " \r\n\t ");

    uloginf("\n\n""\e[34;1m""(((%s)))""\e[0m""\n\n", sh);
    assert(ustrtail(sh)==')');

    char function_name[1024];
    char* function_name_end = strchr(sh, '(');
    assert(function_name_end);
    function_name_end --;
    char* function_name_start = function_name_end - 1 ;
    while(function_name_start >= sh) {
        if(
            (*function_name_start >= 'a' && *function_name_start <= 'z') ||
            (*function_name_start >= 'A' && *function_name_start <= 'Z') ||
            (*function_name_start >= '0' && *function_name_start <= '9') ||
            (*function_name_start == '_')
            ) {
            function_name_start --;
        }
        else {
            break;
        }
    }
    function_name_start ++;
    long size = function_name_end - function_name_start + 1;
    assert(size > 0);
    ustrncpy(function_name, sizeof(function_name), function_name_start, function_name_end - function_name_start + 1);
    assert(strlen(function_name) > 0);
    uloginf("function name : %s\n", function_name);

    char function_type[1024];
    ustrncpy(function_type, sizeof(function_type), sh, function_name_start - sh);
    uloginf("function type : %s\n", function_type);

    ustr_t str;

    str.s = NULL;
    ustr_replaces(function_type, "static", "", &str);
    assert(str.s);
    ustrcpy(function_type, sizeof(function_type), str.s);
    um_free(str.s);

    str.s = NULL;
    ustr_replaces(function_type, "\n", " ", &str);
    assert(str.s);
    ustrcpy(function_type, sizeof(function_type), str.s);
    um_free(str.s);

    while(1) {
        str.s = NULL;
        ustr_replaces(function_type, "  ", " ", &str);
        assert(str.s);
        if(0 == strcmp(str.s, function_type)) {
            um_free(str.s);
            break;
        }
        ustrcpy(function_type, sizeof(function_type), str.s);
        um_free(str.s);
    }

    ustrtrim(function_type);
    uloginf("function type : [%s]\n", function_type);
    ustrcpy(funcp->type, sizeof(funcp->type), function_type);
    assert(strlen(function_type) > 2);

    bool pointer = false;                
    pointer = pointer;
    if('*' == ustrtail(function_type)) {
        pointer = true;
        function_type[strlen(function_type)-1] = '\0';
        ustrtrim(function_type);
    }

    #define NUM_FUNCTION_TYPE_PARSE 10
    ustrw_t strs[NUM_FUNCTION_TYPE_PARSE];
    char s[NUM_FUNCTION_TYPE_PARSE][1024];
    int idx;
    for(idx=0; idx<NUM_FUNCTION_TYPE_PARSE; idx++) {
        strs[idx].wrs = s[idx];
        strs[idx].size = 1024;
    }

    long n = ustrsplit(function_type, " ", 0, NUM_FUNCTION_TYPE_PARSE, strs);
    assert(n <= 2);

    ustrcpy(funcp->name, sizeof(funcp->name), function_name);

    return ret;
}


int parse(struct parsef* f)
{
    int ret = 0; 

    char* start = f->parse;
    char* t = start;
    int at_line = 0;
    int to_line = 0;

    while(t && *t) {
        ulogdbg("At offset %ld :\n", t-start);
        at_line = count_line(f, t-start);
        ulogdbg("At %3d :\n", at_line);

        /*skip blank.*/
        char* skipto = skip_blank(t);
        if(*skipto == '\0') {
            break;
        }
        if(skipto > t) {
            to_line = count_line(f, skipto-start);
            if(to_line == at_line) {
                uloginf("%11d : skip %ld chars\n", at_line, skipto - t);
            }
            else {
                printl(f, t, skipto-1, " blank");
            }

            t = skipto;
            continue;
        }

        if(0 == strncmp(t, "/*", 2)) {
            char* end = strstr(t, "*/");
            assert(end);

            printl(f, t, end, "/**/comment");

            t = end + 2;
            if(*t == '\n') {
                t ++;
            }

            continue;
        }

        if(*t == '#') {
            assert(t==start || *(t-1) == '\n');
            bool cross = false;

            char* cr = NULL;
            char* end = t;
            while(1) {
                cr = strchr(end, '\n');
                assert(cr);

                if(*(cr-1) != '\\') {
                    end = cr + 1;
                    break;
                }
                else {
                    cross = true;
                    cross = cross;
                    end = cr + 1;
                }
            }

            printl(f, t, cr, "#define");

            t = cr+1;
            continue;
        }

        char* stop = ustrchrs(t, "{;");
        assert(stop);

        if(*stop == ';') {
            printl(f, t, stop, ";declare");

            t = stop+1;
            if(*t == '\n') {
                t ++;
            }

            continue;
        }

        if(*stop == '{') {
            char* pair = find_pair(f, stop+1); 
            assert(pair);

            printl(f, t, pair, "{}block");

            char sh[1024];
            ustrncpy(sh, sizeof(sh), t, stop -t);

            ustrtrim_chars(sh, true, true, " \r\n\t");

            char ch = *(pair+1);

            if(ustrtail(sh) == '=') {
                assert(ch == ';');
            }
            else if(0 == strncmp(sh, "typedef", strlen("typedef"))){


            }
            else if(0 == strncmp(sh, "struct", strlen("struct"))) {
                assert(ch == ';');

            }
            else {
                struct function_property funcp;
                function_define_parse(f, t, stop, pair, &funcp);

                f->number_function ++;

                bool monitor = !function_not_monitor(funcp.name);
                bool detail = !function_not_detail(funcp.name);
                detail = detail;

                /* add ENTER__. */
                struct urange range = { stop - f->parse + 1, 0 };
                replace_range_add(f, range, "ENTER__", monitor);

                /* return. */
                long offset = stop - start;
                char* function_content = ustrndup(stop, pair-stop+1);
                //uloginf("\n\n""\e[36;1m""(((%s)))""\e[0m""\n\n", function_content);

                char* t = function_content;
                int number_return = 0;
                bool return_void = false;
                while(1) {
                    char* r = strstr(t, "return");
                    if(!r) {
                        break;
                    }

                    char* end = strchr(r, ';');
                    assert(end);

                    bool is_return_function = false;

                    char return_statment[1024];
                    ustrncpy(return_statment, sizeof(return_statment), r+strlen("return"), end - (r+strlen("return")));

                    ustrtrim(return_statment);
                    struct urange range_parse_return_statment[10];
                    long n = ustr_ranges(return_statment, "(", ")", range_parse_return_statment, 10);
                    if(n == 0) {

                    }
                    else if(n == 1 && range_parse_return_statment[0].location == 0) {

                    }
                    else {
                        uloginf("return [\e[36;2m%s\e[0m]\n", return_statment);
                        kstatics.number_return_function ++;
                        is_return_function = true;
                    }

                    char return_stament[1024];
                    ustrncpy(return_stament, sizeof(return_stament), r, end-r+1);


                    if(r[6] == ';' || r[6] == ' ') {
                        if(is_return_function) {
                            char tmp[1024];
                            snprintf(tmp, sizeof(tmp), "RETURN_TYPE__(%s,", funcp.type);
                            char* dup = ustrdup(tmp);
                            addto_delayfree(f, dup);
                            
                            struct urange range = {
                                r-function_content+offset, 6
                            };
                            replace_range_add(f, range, dup, monitor);

                            struct urange range_end = {
                                end-function_content+offset, 0
                            };
                            replace_range_add(f, range_end, " RETURN_TYPE_END__)", monitor);
                        }
                        else {
                            struct urange range = {
                                r-function_content+offset, 6
                            };
                            replace_range_add(f, range, "RETURN__", monitor);

                            struct urange range_end = {
                                end+1-function_content+offset, 0
                            };
                            replace_range_add(f, range_end, "RETURN_END__", monitor);
                        }
                        
                        number_return ++;

                        if(r[6] == ';') {
                            return_void = true;    
                        }
                    }
                    else {
                        uloginf("\n\n""\e[31;1m""(((%s)))""\e[0m""\n\n", return_stament);
                        //read(0, return_stament, 10);
                    }

                    t = end+1;
                }

                if(number_return == 0 || return_void) {
                    /* check function return type void. */
                    struct urange range = {
                        pair-start, 0
                    };
                    replace_range_add(f, range, "RETURN_VOID_ADD__", monitor);
                }

                um_free(function_content);
            }

            t = pair + 1;

            if(*t == '\n') {
                t ++;
            }
            
            continue;
        }
        
        char s100[100];
        ustrcpy(s100, 100, t);
        uloginf("\tNot recognized.(%s)\n", s100);
        break;
        char* tmp = strchr(t, '\n');
        if(tmp) {
            t = tmp + 1;
        }
        else {
            break;
        }

        continue;
    }

    kstatics.number_function += f->number_function;

    ustr_t str;
    int retr = ustr_replace_members(f->content, 
        f->nreplace_members, 
        f->replace_members, 
    	&str);
    assert(retr == 0);

    ufile_write(f->filename, "w", str.s, strlen(str.s));
    um_free(str.s);


    uloginf("number_function : %d\n", f->number_function);
    uloginf("finish parse.  ---%s---\n\n\n", f->filename);

    return ret;
}


bool file_enable_monitor(const char* filename)
{
    bool ret = true;

    char** pps = kfiles_not_monitor;
    while(*pps) {
        if(strstr(filename, *pps)) {
            return false;
        }

        pps ++;
    }

    return ret;
}


int parse_file(const char* filename, int sn)
{
    int ret = 0;

    uloginf("parse [%d]%s\n", sn, filename);

    if(!file_enable_monitor(filename)) {
        uloginf("not monitor [%d]%s\n\n\n", sn, filename);
        return ret;
    }


    /* clear previous ENTER___, RETURN. */
    clear_enter_return(filename); 

    /* read file content. */
    struct parsef* f = read_file(filename);

    /* read line. */
    parse_file_line(f);

    /* clear comment. */
    remove_comments(f); 

    /* clear some define interupt to function name or parameter */
    remove_need_clear(f);

    /* remove strings. */
    remove_cstrings(f);

    /* connect line. */
    connect_cr(f);

    /* clear define. */
    remove_define(f);

    parse(f);

    parse_free(f);
    f = NULL;

    return ret;
}

int test(void);
int dfc(int x);


#ifndef TEST
int main(int argc, char* argv[])
{
    uloginf("total file number : %d\n", argc-1);

    int umalloc_enable(int set); umalloc_enable(true);
    int idx;
    for(idx=1; idx<argc; idx++) {
        char* filename = argv[idx];
        parse_file(filename, idx);
    }

    uloginf("total number function : %d\n", kstatics.number_function);
    uloginf("total number return function : %d\n", kstatics.number_return_function);
    uloginf("finish.\n");

    return 0;
}

#else

int main(int argc, char* argv[])
{
    //test();
    int umalloc_enable(int set);
    umalloc_enable(true);
    char qs[1024];

    while(1) {
    char* s = um_malloc(1024);

    umalloc_query(qs, sizeof(qs));
    uloginf("------\n%s\n", qs);

    um_free(s);
    umalloc_query(qs, sizeof(qs));
    uloginf("------\n%s\n", qs);
    }


    return 0;
}
#endif







