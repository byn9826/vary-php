#include "php.h"
#include "Zend/zend_smart_str.h"
#include "ext/pdo/php_pdo_driver.h"

zend_class_entry *_conn_handle;

PHP_METHOD(_conn, _getConn)
{
  zval *_conn = zend_read_static_property(_conn_handle, "__conn__", sizeof("__conn__") - 1, 0);
  zval conn;
  ZVAL_COPY(&conn, _conn);
  if (!ZVAL_IS_NULL(&conn)) {
    RETURN_ZVAL(&conn, 0, 1);
  }
  zval *_driver = zend_read_static_property(_conn_handle, "__driver__", sizeof("__driver__") - 1, 1);
  zval *_host = zend_read_static_property(_conn_handle, "__host__", sizeof("__host__") - 1, 1);
  zval *_port = zend_read_static_property(_conn_handle, "__port__", sizeof("__port__") - 1, 1);
  zval *_database = zend_read_static_property(_conn_handle, "__database__", sizeof("__database__") - 1, 1);
  zval *_username = zend_read_static_property(_conn_handle, "__username__", sizeof("__username__") - 1, 1);
  zval *_password = zend_read_static_property(_conn_handle, "__password__", sizeof("__password__") - 1, 1);
  zval driver, host, database;
  ZVAL_NEW_STR(&driver, zval_get_string(_driver));
  ZVAL_NEW_STR(&host, zval_get_string(_host));
  ZVAL_NEW_STR(&database, zval_get_string(_database));
  smart_str conn_string = {0};
  smart_str_appends(&conn_string, Z_STRVAL(driver));
  smart_str_appends(&conn_string, ":host=");
  smart_str_appends(&conn_string, Z_STRVAL(host));
  smart_str_appends(&conn_string, ";dbname=");
  smart_str_appends(&conn_string, Z_STRVAL(database));
  smart_str_appends(&conn_string, ";port=");
  convert_to_string(_port);
  smart_str_appends(&conn_string, Z_STRVAL_P(_port));
  smart_str_0(&conn_string);
  zval_ptr_dtor(&driver);
  zval_ptr_dtor(&host);
  zval_ptr_dtor(&database);
  zval pdo_init_name, pdo_init_retval;
  ZVAL_STRING(&pdo_init_name, "__construct");
  zval params[3];
  ZVAL_STR(&params[0], conn_string.s);
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
  zend_update_static_property(
    _conn_handle,
    "__conn__",
    sizeof("__conn__") - 1,
    &obj TSRMLS_CC
  );
  RETURN_ZVAL(&obj, 0, 1);
}