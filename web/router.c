#include "php.h"

zend_class_entry *router_handle;

PHP_METHOD(Router, get)
{
  zend_string *_uri, *_controller, *_method;
  ZEND_PARSE_PARAMETERS_START(3, 3)
    Z_PARAM_STR(_uri)
    Z_PARAM_STR(_controller)
    Z_PARAM_STR(_method)
  ZEND_PARSE_PARAMETERS_END();
  zval *_rules = zend_read_static_property(router_handle, "__rules__", sizeof("__rules__") - 1, 1);
  zval rules;
  ZVAL_COPY(&rules, _rules);
  if (ZVAL_IS_NULL(&rules)) {
    array_init(&rules);
  }
  zval strtolower_name, strtolower_retval, uri;
  ZVAL_STR(&uri, _uri);
  ZVAL_STRING(&strtolower_name, "strtolower");
  call_user_function(
    EG(function_table),
    NULL,
    &strtolower_name,
    &strtolower_retval,
    1,
    &uri TSRMLS_CC
  );
  zval_ptr_dtor(&strtolower_name);
  zval_ptr_dtor(&uri);
  zval explode_name, explode_retval;
  ZVAL_STRING(&explode_name, "explode");
  zval params[2];
  ZVAL_STRINGL(&params[0], "/", sizeof("/") - 1);
  ZVAL_COPY(&params[1], &strtolower_retval);
  zval_ptr_dtor(&strtolower_retval);
  call_user_function(
    EG(function_table),
    NULL,
    &explode_name,
    &explode_retval,
    2,
    params TSRMLS_CC
  );
  zval_ptr_dtor(&explode_name);
  zval_ptr_dtor(&params[0]);
  zval_ptr_dtor(&params[1]);
  smart_str _final_url = {0};
  zend_long variables_nums = 0;
  for (zend_long i = 1; i < zend_hash_num_elements(Z_ARRVAL(explode_retval)); ++i) {
    Bucket *carry = Z_ARRVAL(explode_retval)->arData + i;
    smart_str_appends(&_final_url, "/");
    if (Z_STRVAL(carry->val)[0] == '{' && Z_STRVAL(carry->val)[Z_STRLEN(carry->val) - 1] == '}') {
      smart_str_appends(&_final_url, "{");
      zval variable;
      ZVAL_LONG(&variable, variables_nums);
      convert_to_string(&variable);
      smart_str_appends(&_final_url, Z_STRVAL(variable));
      smart_str_appends(&_final_url, "}");
      ++variables_nums;
    } else {
      smart_str_appends(&_final_url, Z_STRVAL(carry->val));
    }
  }
  smart_str_0(&_final_url);
  zval_ptr_dtor(&explode_retval);
  zval final_url;
  ZVAL_STR(&final_url, _final_url.s);
  zval handler, controller, method;
  array_init(&handler);
  ZVAL_STR(&controller, _controller);
  ZVAL_STR(&method, _method);
  zend_hash_next_index_insert(Z_ARRVAL(handler), &controller);
  zend_hash_next_index_insert(Z_ARRVAL(handler), &method);
  zend_hash_str_update(
    Z_ARRVAL(rules),
    Z_STRVAL(final_url),
    Z_STRLEN(final_url),
    &handler
  );
  zval_ptr_dtor(&final_url);
  zend_update_static_property(
    router_handle,
    "__rules__",
    sizeof("__rules__") - 1,
    &rules TSRMLS_CC
  );
  zval_ptr_dtor(&rules);
}

PHP_METHOD(Router, handle)
{
  zend_string *_uri;
  ZEND_PARSE_PARAMETERS_START(1, 1)
    Z_PARAM_STR(_uri)
  ZEND_PARSE_PARAMETERS_END();
  zval *_rules = zend_read_static_property(router_handle, "__rules__", sizeof("__rules__") - 1, 1);
  zval *_matched = zend_hash_find(Z_ARRVAL_P(_rules), _uri);
  if (_matched) {
    RETURN_ZVAL(_matched, 1, 0);
  }
  zval explode_name, explode_retval;
  ZVAL_STRING(&explode_name, "explode");
  zval params[2];
  ZVAL_STRING(&params[0], "/");
  ZVAL_STR(&params[1], _uri);
  call_user_function(
    EG(function_table),
    NULL,
    &explode_name,
    &explode_retval,
    2,
    params TSRMLS_CC
  );
  zval_ptr_dtor(&explode_name);
  zval_ptr_dtor(&params[0]);
  zval_ptr_dtor(&params[1]);
  zend_long array_size = zend_hash_num_elements(Z_ARRVAL(explode_retval));
  zend_long padding_index = array_size - 1;
  zend_long match_find = 0;
  while (padding_index > 1 && match_find == 0) {
    smart_str _final_url = {0};
    for (zend_long i = 1; i < array_size; ++i) {
      smart_str_appends(&_final_url, "/");
      Bucket *carry = Z_ARRVAL(explode_retval)->arData + i;
      if (i >= padding_index) {
        smart_str_appends(&_final_url, "{");
        zval variable;
        ZVAL_LONG(&variable, i - padding_index);
        convert_to_string(&variable);
        smart_str_appends(&_final_url, Z_STRVAL(variable));
        smart_str_appends(&_final_url, "}");
      } else {
        smart_str_appends(&_final_url, Z_STRVAL(carry->val));
      }
    }
    smart_str_0(&_final_url);
    TRACE("Final: %s", ZSTR_VAL(_final_url.s));
    zval final_url;
    ZVAL_STR(&final_url, _final_url.s);
    _matched = zend_hash_find(Z_ARRVAL_P(_rules), Z_STR(final_url));
    zval_ptr_dtor(&final_url);
    if (_matched) {
      match_find = 1;
    }
    --padding_index;
  }
  zval_ptr_dtor(&explode_retval);
  if (match_find == 1) {
    RETURN_ZVAL(_matched, 1, 0);
  } else {
    RETURN_FALSE;
  }
}