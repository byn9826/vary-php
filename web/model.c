#include "php.h"
#include "Zend/zend_smart_str.h"
#include "ext/pdo/php_pdo_driver.h"

zend_class_entry *model_handle;

PHP_METHOD(Model, config) {}

PHP_METHOD(Model, fetch)
{
  
  zval *_conn = zend_read_static_property(model_handle, "__connection__", sizeof("__connection__") - 1, 1);
  zval conn;
  ZVAL_ZVAL(&conn, _conn, 0, 0);
  zval_ptr_dtor(&conn);
  // if (Z_ISNULL(conn)) {
  //   conn = vary_model_getConnection();
  // }
  // RETURN_ZVAL(&conn, 0, 1);
}

PHP_METHOD(Model, __construct)
{
  
}

PHP_METHOD(Model, useTable)
{
  zend_string *_name;
  ZEND_PARSE_PARAMETERS_START(1, 3)
    Z_PARAM_STR(_name)
  ZEND_PARSE_PARAMETERS_END();
  zend_update_static_property_string(
    model_handle,
    "__table__",
    sizeof("__table__") - 1,
    ZSTR_VAL(_name)
  );
}

