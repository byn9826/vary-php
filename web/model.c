#include "php.h"
#include "ext/pdo/php_pdo_driver.h"

zend_class_entry *model_handle;

static zval vary_model_prepare(zend_string *statement)
{
  zval conn = vary_conn_getConnection();
  zval params[1];
  ZVAL_STR(&params[0], statement);
  zval model_prepare_name, model_prepare_retval;
  ZVAL_STRING(&model_prepare_name, "prepare");
  call_user_function(
    EG(function_table),
    &conn,
    &model_prepare_name,
    &model_prepare_retval,
    1,
    params TSRMLS_CC
  );
  zval_ptr_dtor(&conn);
  zval_ptr_dtor(&params[0]);
  zval_ptr_dtor(&model_prepare_name);
  return model_prepare_retval;
}

static zval vary_model_execute(zval statement)
{
  zval model_execute_name, model_execute_retval;
  ZVAL_STRING(&model_execute_name, "execute");
  call_user_function(
    EG(function_table),
    &statement,
    &model_execute_name,
    &model_execute_retval,
    0,
    NULL TSRMLS_CC
  );
  zval_ptr_dtor(&model_execute_name);
  zval_ptr_dtor(&model_execute_retval);
}

static zval vary_model_fetchAll(zval statement, zend_long type)
{
  zval model_fetchAll_name, model_fetchAll_retval;
  ZVAL_STRING(&model_fetchAll_name, "fetchAll");
  if (type == 0) {
    call_user_function(
      EG(function_table),
      &statement,
      &model_fetchAll_name,
      &model_fetchAll_retval,
      0,
      NULL TSRMLS_CC
    );
  } else {
    zval args[1];
    ZVAL_LONG(&args[0], PDO_FETCH_COLUMN);
    call_user_function(
      EG(function_table),
      &statement,
      &model_fetchAll_name,
      &model_fetchAll_retval,
      1,
      args TSRMLS_CC
    );
    zval_ptr_dtor(&args[0]);
  }
  zval_ptr_dtor(&statement);
  zval_ptr_dtor(&model_fetchAll_name);
  return model_fetchAll_retval;
}

PHP_METHOD(Model, config) {}

PHP_METHOD(Model, useTable)
{
  zend_string *_name;
  ZEND_PARSE_PARAMETERS_START(1, 3)
    Z_PARAM_STR(_name)
  ZEND_PARSE_PARAMETERS_END();
  zval *_table_name = zend_read_static_property(
    zend_get_called_scope(execute_data),
    "__table__",
    sizeof("__table__") - 1,
    1
  );
  if (zval_get_type(_table_name) != IS_STRING) {
    zval name;
    ZVAL_STR(&name, _name);
    zend_update_static_property(
      zend_get_called_scope(execute_data),
      "__table__",
      sizeof("__table__") - 1,
      &name TSRMLS_CC
    );
    smart_str _model_prepare_string = {0};
    smart_str_appends(&_model_prepare_string, "DESCRIBE ");
    smart_str_appends(&_model_prepare_string, Z_STRVAL(name));
    smart_str_0(&_model_prepare_string);
    zval_ptr_dtor(&name);
    zval statement = vary_model_prepare(_model_prepare_string.s);
    vary_model_execute(statement);
    zval columns = vary_model_fetchAll(statement, 1);
    zend_update_static_property(
      zend_get_called_scope(execute_data),
      "__columns__",
      sizeof("__columns__") - 1,
      &columns TSRMLS_CC
    );
    zval_ptr_dtor(&columns);
  }
}

PHP_METHOD(Model, list)
{
  zval *_limit;
  ZEND_PARSE_PARAMETERS_START(0, 1)
    Z_PARAM_OPTIONAL
    Z_PARAM_ZVAL(_limit)
  ZEND_PARSE_PARAMETERS_END();
  zend_string *_name = zend_get_called_scope(execute_data)->name;
  smart_str _model_config_name = {0};
  smart_str_appends(&_model_config_name, ZSTR_VAL(_name));
  smart_str_appends(&_model_config_name, "::config");
  smart_str_0(&_model_config_name);
  zval model_config_name, model_config_retval;
  ZVAL_STR(&model_config_name, _model_config_name.s);
  call_user_function(
    EG(function_table),
    NULL,
    &model_config_name,
    &model_config_retval,
    0,
    NULL TSRMLS_CC
  );
  zval_ptr_dtor(&model_config_name);
  zval_ptr_dtor(&model_config_retval);
  zval *_table_name = zend_read_static_property(zend_get_called_scope(execute_data), "__table__", sizeof("__table__") - 1, 1);
  smart_str _model_prepare_string = {0};
  smart_str_appends(&_model_prepare_string, "SELECT * FROM ");
  smart_str_appends(&_model_prepare_string, Z_STRVAL_P(_table_name));
  if (ZEND_NUM_ARGS() == 1) {
    smart_str_appends(&_model_prepare_string, " LIMIT ");
    convert_to_string(_limit)
    smart_str_appends(&_model_prepare_string, Z_STRVAL_P(_limit));
  }
  smart_str_0(&_model_prepare_string);
  zval statement = vary_model_prepare(_model_prepare_string.s);
  zval model_mode_name, model_mode_retval;
  ZVAL_STRING(&model_mode_name, "setFetchMode");
  zval args[2];
  ZVAL_LONG(&args[0], PDO_FETCH_CLASS | PDO_FETCH_PROPS_LATE);
  ZVAL_STR(&args[1], _name);
  call_user_function(
    EG(function_table),
    &statement,
    &model_mode_name,
    &model_mode_retval,
    2,
    args TSRMLS_CC
  );
  zval_ptr_dtor(&model_mode_name);
  zval_ptr_dtor(&model_mode_retval);
  zval_ptr_dtor(&args[0]);
  zval_ptr_dtor(&args[1]);
  vary_model_execute(statement);
  zval result = vary_model_fetchAll(statement, 0);
  RETURN_ARR(Z_ARRVAL(result));
}

PHP_METHOD(Model, __construct)
{
  
}

