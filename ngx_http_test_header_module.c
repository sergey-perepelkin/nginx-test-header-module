#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_http.h>

static ngx_str_t target_header_name = ngx_null_string;

static char *ngx_http_test_header(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);
static ngx_int_t ngx_http_test_header_handler(ngx_http_request_t *r);
static ngx_int_t ngx_http_test_init(ngx_conf_t *cf);

static ngx_command_t ngx_http_test_header_commands[] = {

    { ngx_string("test_header"),
      NGX_HTTP_LOC_CONF | NGX_CONF_TAKE1,
      ngx_http_test_header,
      0,
      0,
      NULL },

    ngx_null_command /* command termination */
};

static ngx_http_module_t ngx_http_test_header_module_ctx = {
    NULL,               /* preconfiguration */
    ngx_http_test_init, /* postconfiguration */

    NULL,               /* create main configuration */
    NULL,               /* init main configuration */

    NULL,               /* create server configuration */
    NULL,               /* merge server configuration */

    NULL,               /* create location configuration */
    NULL                /* merge location configuration */
};

ngx_module_t ngx_http_test_header_module = {
    NGX_MODULE_V1,
    &ngx_http_test_header_module_ctx, /* module context */
    ngx_http_test_header_commands,    /* module directives */
    NGX_HTTP_MODULE,                  /* module type */
    NULL,                             /* init master */
    NULL,                             /* init module */
    NULL,                             /* init process */
    NULL,                             /* init thread */
    NULL,                             /* exit thread */
    NULL,                             /* exit process */
    NULL,                             /* exit master */
    NGX_MODULE_V1_PADDING
};

static ngx_int_t
ngx_http_test_header_handler(ngx_http_request_t *r) {
    ngx_table_elt_t *header;
    char            *random_value_str;
    ngx_str_t        value_str;
    int              random_number;

    srand((unsigned) ngx_time());

    random_number = rand() % 100;
    random_value_str = ngx_pnalloc(r->pool, NGX_INT32_LEN + 1);
    if (random_value_str == NULL) {
        return NGX_ERROR;
    }
    ngx_sprintf((u_char *)random_value_str, "%d", random_number);

    value_str.data = (u_char *)random_value_str;
    value_str.len = ngx_strlen(value_str.data);

    header = ngx_list_push(&r->headers_out.headers);
    if (header == NULL) {
        return NGX_ERROR;
    }

    header->hash = 1;
    header->key.len =    target_header_name.len;
    header->key.data =   target_header_name.data;
    header->value.len =  value_str.len;
    header->value.data = value_str.data;

    return NGX_DECLINED;
}

static char
*ngx_http_test_header(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
    ngx_str_t *value = cf->args->elts;
    
    if (cf->args->nelts != 2) {
        return "Invalid number of arguments for 'test_header'";
    }

    target_header_name = value[1];

    return NGX_CONF_OK;
}


static ngx_int_t
ngx_http_test_init(ngx_conf_t *cf)
{
    ngx_http_handler_pt        *h;
    ngx_http_core_main_conf_t  *cmcf;

    cmcf = ngx_http_conf_get_module_main_conf(cf, ngx_http_core_module);

    h = ngx_array_push(&cmcf->phases[NGX_HTTP_POST_READ_PHASE].handlers);
    if (h == NULL) {
        return NGX_ERROR;
    }

    *h = ngx_http_test_header_handler;

    return NGX_OK;
}
