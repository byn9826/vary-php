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
  zval strtolower_name, strtolower_retval;
  zval uri;
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
  if (!zend_symtable_exists_ind(Z_ARRVAL(rules), Z_STR(strtolower_retval))) {
    zval rule_set;
    array_init(&rule_set);
    zval controller, method;
    ZVAL_STR(&controller, _controller);
    ZVAL_STR(&method, _method);
    zend_hash_next_index_insert(Z_ARRVAL(rule_set), &controller);
    zend_hash_next_index_insert(Z_ARRVAL(rule_set), &method);
    zend_hash_str_add_new(
      Z_ARRVAL(rules),
      Z_STRVAL(strtolower_retval),
      Z_STRLEN(strtolower_retval),
      &rule_set
    );
    zend_update_static_property(
      router_handle,
      "__rules__",
      sizeof("__rules__") - 1,
      &rules TSRMLS_CC
    );
  }
  zval_ptr_dtor(&rules);
  zval_ptr_dtor(&strtolower_retval);
}