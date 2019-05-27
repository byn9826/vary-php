#include "php.h"
#include "Zend/zend_smart_str.h"
#include "ext/pdo/php_pdo_driver.h"

zend_class_entry *conn_handle;

zval vary_conn_getConnection()
{
  zval *_conn = zend_read_static_property(conn_handle, "__conn__", sizeof("__conn__") - 1, 1);
  zval conn;
  ZVAL_COPY(&conn, _conn);
  if (!ZVAL_IS_NULL(&conn)) {
    return conn;
  }
  zval *_host = zend_read_static_property(conn_handle, "__host__", sizeof("__host__") - 1, 1);
  zval *_port = zend_read_static_property(conn_handle, "__port__", sizeof("__port__") - 1, 1);
  zval *_database = zend_read_static_property(conn_handle, "__database__", sizeof("__database__") - 1, 1);
  zval *_username = zend_read_static_property(conn_handle, "__username__", sizeof("__username__") - 1, 1);
  zval *_password = zend_read_static_property(conn_handle, "__password__", sizeof("__password__") - 1, 1);
  zval host, database;
  ZVAL_NEW_STR(&host, zval_get_string(_host));
  ZVAL_NEW_STR(&database, zval_get_string(_database));
  smart_str conn_string = {0};
  smart_str_appends(&conn_string, "mysql:host=");
  smart_str_appends(&conn_string, Z_STRVAL(host));
  smart_str_appends(&conn_string, ";dbname=");
  smart_str_appends(&conn_string, Z_STRVAL(database));
  smart_str_appends(&conn_string, ";port=");
  convert_to_string(_port);
  smart_str_appends(&conn_string, Z_STRVAL_P(_port));
  smart_str_0(&conn_string);
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
    conn_handle,
    "__conn__",
    sizeof("__conn__") - 1,
    &obj TSRMLS_CC
  );
  return obj;
}

PHP_METHOD(Conn, getConn)
{
  ZEND_PARSE_PARAMETERS_START(0, 0)
  ZEND_PARSE_PARAMETERS_END();
  zval conn = vary_conn_getConnection();
  RETURN_ZVAL(&conn, 0, 1);
}

PHP_METHOD(Conn, setConn)
{
  zval *_array;
  ZEND_PARSE_PARAMETERS_START(1, 1)
    Z_PARAM_ARRAY(_array)
  ZEND_PARSE_PARAMETERS_END();
  zval host_key, *host_value;
  ZVAL_STRING(&host_key, "host");
  host_value = zend_hash_find(Z_ARRVAL_P(_array), Z_STR(host_key));
  zval_ptr_dtor(&host_key);
  zend_update_static_property(
    zend_get_called_scope(execute_data),
    "__host__",
    sizeof("__host__") - 1,
    host_value TSRMLS_CC
  );
  zval port_key, *port_value;
  ZVAL_STRING(&port_key, "port");
  port_value = zend_hash_find(Z_ARRVAL_P(_array), Z_STR(port_key));
  zval_ptr_dtor(&port_key);
  if (port_value) {
    zend_update_static_property(
      zend_get_called_scope(execute_data),
      "__port__",
      sizeof("__port__") - 1,
      port_value TSRMLS_CC
    );
  }
  zval database_key, *database_value;
  ZVAL_STRING(&database_key, "database");
  database_value = zend_hash_find(Z_ARRVAL_P(_array), Z_STR(database_key));
  zval_ptr_dtor(&database_key);
  zend_update_static_property(
    zend_get_called_scope(execute_data),
    "__database__",
    sizeof("__database__") - 1,
    database_value TSRMLS_CC
  );
  zval username_key, *username_value;
  ZVAL_STRING(&username_key, "username");
  username_value = zend_hash_find(Z_ARRVAL_P(_array), Z_STR(username_key));
  zval_ptr_dtor(&username_key);
  if (username_value) {
    zend_update_static_property(
      zend_get_called_scope(execute_data),
      "__username__",
      sizeof("__username__") - 1,
      username_value TSRMLS_CC
    );
  }
  zval password_key, *password_value;
  ZVAL_STRING(&password_key, "password");
  password_value = zend_hash_find(Z_ARRVAL_P(_array), Z_STR(password_key));
  zval_ptr_dtor(&password_key);
  if (password_value) {
    zend_update_static_property(
      zend_get_called_scope(execute_data),
      "__password__",
      sizeof("__password__") - 1,
      password_value TSRMLS_CC
    );
  }
}