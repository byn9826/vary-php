#include "php.h"
#include "Zend/zend_smart_str.h"
#include "ext/pdo/php_pdo_driver.h"

zend_class_entry *model_handle;

PHP_METHOD(Model, getConnection)
{
  zval *_type = zend_read_static_property(model_handle, "__type__", sizeof("__type__") - 1, 0);
  zval *_host = zend_read_static_property(model_handle, "__host__", sizeof("__host__") - 1, 0);
  zval *_port = zend_read_static_property(model_handle, "__port__", sizeof("__port__") - 1, 0);
  zval *_database = zend_read_static_property(model_handle, "__database__", sizeof("__database__") - 1, 0);
  zval *_username = zend_read_static_property(model_handle, "__username__", sizeof("__username__") - 1, 0);
  zval *_password = zend_read_static_property(model_handle, "__password__", sizeof("__password__") - 1, 0);
  zval *_connection = zend_read_static_property(model_handle, "__connection__", sizeof("__connection__") - 1, 0);
  smart_str connection_string = {0};
  smart_str_appends(&connection_string, Z_STRVAL_P(_type));
  smart_str_appends(&connection_string, ":host=");
  smart_str_appends(&connection_string, Z_STRVAL_P(_host));
  smart_str_appends(&connection_string, ";dbname=");
  smart_str_appends(&connection_string, Z_STRVAL_P(_database));
  smart_str_appends(&connection_string, ";port=");
  convert_to_string(_port);
  smart_str_appends(&connection_string, Z_STRVAL_P(_port));
  smart_str_0(&connection_string);
  zval pdo_init_name, pdo_init_retval;
  ZVAL_STRING(&pdo_init_name, "__construct");
  zval params[3];
  ZVAL_STR(&params[0], connection_string.s);
  ZVAL_STR(&params[1], zval_get_string(_username));
  ZVAL_STR(&params[2], zval_get_string(_password));
  zval obj;
  object_init_ex(&obj, php_pdo_get_dbh_ce());
  call_user_function(
    EG(function_table),
    &obj,
    &pdo_init_name,
    &pdo_init_retval,
    3,
    params TSRMLS_CC
  );
  zval_ptr_dtor(&params[0]);
  zval_ptr_dtor(&params[1]);
  zval_ptr_dtor(&params[2]);
  zval_ptr_dtor(&pdo_init_name);
  zval_ptr_dtor(&pdo_init_retval);
  RETURN_ZVAL(&obj, 0, 1);
}