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
  zval rule_set;
  zend_long rule_exist = 0;
  if (!zend_symtable_exists_ind(Z_ARRVAL(rules), _controller)) {
    array_init(&rule_set);
  } else {
    zval *_rule_set = zend_hash_find(Z_ARRVAL(rules), _controller);
    rule_exist = 1;
    ZVAL_COPY(&rule_set, _rule_set);
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
  zval method;
  ZVAL_STR(&method, _method);
  if (zend_symtable_exists_ind(Z_ARRVAL(rule_set), Z_STR(strtolower_retval))) {
    zend_hash_str_update(
      Z_ARRVAL(rule_set),
      Z_STRVAL(strtolower_retval),
      Z_STRLEN(strtolower_retval),
      &method
    );
  } else {
    zend_hash_str_add_new(
      Z_ARRVAL(rule_set),
      Z_STRVAL(strtolower_retval),
      Z_STRLEN(strtolower_retval),
      &method
    );
  }
  if (rule_exist == 1) {
    zend_hash_str_update(
      Z_ARRVAL(rules),
      ZSTR_VAL(_controller),
      ZSTR_LEN(_controller),
      &rule_set
    );
  } else {
    zend_hash_str_add_new(
      Z_ARRVAL(rules),
      ZSTR_VAL(_controller),
      ZSTR_LEN(_controller),
      &rule_set
    );
  }
  zend_update_static_property(
    router_handle,
    "__rules__",
    sizeof("__rules__") - 1,
    &rules TSRMLS_CC
  );
  zval_ptr_dtor(&rules);
  zval_ptr_dtor(&strtolower_retval);
}

PHP_METHOD(Router, handle)
{
  zend_string *_uri;
  ZEND_PARSE_PARAMETERS_START(1, 1)
    Z_PARAM_STR(_uri)
  ZEND_PARSE_PARAMETERS_END();
  zval explode_name, explode_retval;
  ZVAL_STRING(&explode_name, "explode");
  zval params[2];
  ZVAL_STRINGL(&params[0], "/", sizeof("/") - 1);
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
  RETURN_ZVAL(&explode_retval, 0, 1);
}