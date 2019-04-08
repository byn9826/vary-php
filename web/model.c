#include "php.h"

zend_class_entry *model_handle;

PHP_METHOD(Model, config) {}

PHP_METHOD(Model, useTable)
{
  zend_string *_name;
  ZEND_PARSE_PARAMETERS_START(1, 3)
    Z_PARAM_STR(_name)
  ZEND_PARSE_PARAMETERS_END();
  zval name;
  ZVAL_STR(&name, _name);
  zend_update_static_property(
    zend_get_called_scope(execute_data),
    "__table__",
    sizeof("__table__") - 1,
    &name TSRMLS_CC
  );
  zval_ptr_dtor(&name);
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
  zval conn = vary_model_getConn();
  smart_str _model_prepare_string = {0};
  smart_str_appends(&_model_prepare_string, "SELECT * FROM ");
  smart_str_appends(&_model_prepare_string, Z_STRVAL_P(_table_name));
  if (ZEND_NUM_ARGS() == 1) {
    smart_str_appends(&_model_prepare_string, " LIMIT ");
    convert_to_string(_limit)
    smart_str_appends(&_model_prepare_string, Z_STRVAL_P(_limit));
  }
  smart_str_0(&_model_prepare_string);
  zval params[1];
  ZVAL_STR(&params[0], _model_prepare_string.s);
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
  zval model_execute_name, model_execute_retval;
  ZVAL_STRING(&model_execute_name, "execute");
  call_user_function(
    EG(function_table),
    &model_prepare_retval,
    &model_execute_name,
    &model_execute_retval,
    0,
    NULL TSRMLS_CC
  );
  zval_ptr_dtor(&model_execute_name);
  zval_ptr_dtor(&model_execute_retval);
  zval model_fetchAll_name, model_fetchAll_retval;
  ZVAL_STRING(&model_fetchAll_name, "fetchAll");
  call_user_function(
    EG(function_table),
    &model_prepare_retval,
    &model_fetchAll_name,
    &model_fetchAll_retval,
    0,
    NULL TSRMLS_CC
  );
  zval_ptr_dtor(&model_fetchAll_name);
  zval_ptr_dtor(&model_prepare_retval);
  RETURN_ARR(Z_ARRVAL(model_fetchAll_retval));
}

PHP_METHOD(Model, __construct)
{
  
}

