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
  ZEND_PARSE_PARAMETERS_START(1, 1)
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

PHP_METHOD(Model, usePrimary)
{
  zend_string *_name;
  ZEND_PARSE_PARAMETERS_START(1, 1)
    Z_PARAM_STR(_name)
  ZEND_PARSE_PARAMETERS_END();
  zval *_table_name = zend_read_static_property(
    zend_get_called_scope(execute_data),
    "__primary__",
    sizeof("__primary__") - 1,
    1
  );
  if (zval_get_type(_table_name) != IS_STRING) {
    zval name;
    ZVAL_STR(&name, _name);
    zend_update_static_property(
      zend_get_called_scope(execute_data),
      "__primary__",
      sizeof("__primary__") - 1,
      &name TSRMLS_CC
    );
    zval_ptr_dtor(&name);
  }
}

PHP_METHOD(Model, where)
{
  zval list, *_list;
  ZEND_PARSE_PARAMETERS_START(1, 1)
    Z_PARAM_ARRAY(_list)
  ZEND_PARSE_PARAMETERS_END();
  ZVAL_COPY(&list, _list);
  zend_update_static_property(
    zend_get_called_scope(execute_data),
    "__where__",
    sizeof("__where__") - 1,
    &list TSRMLS_CC
  );
  zval_ptr_dtor(&list);
}

PHP_METHOD(Model, list)
{
  zval *_limit, *_offset;
  ZEND_PARSE_PARAMETERS_START(0, 2)
    Z_PARAM_OPTIONAL
    Z_PARAM_ZVAL(_limit)
    Z_PARAM_ZVAL(_offset)
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
  zval *_table_name = zend_read_static_property(
    zend_get_called_scope(execute_data),
    "__table__",
    sizeof("__table__") - 1,
    1
  );
  smart_str _model_prepare_string = {0};
  smart_str_appends(&_model_prepare_string, "SELECT * FROM ");
  smart_str_appends(&_model_prepare_string, Z_STRVAL_P(_table_name));
  if (ZEND_NUM_ARGS() >= 1) {
    smart_str_appends(&_model_prepare_string, " LIMIT ");
    convert_to_string(_limit)
    smart_str_appends(&_model_prepare_string, Z_STRVAL_P(_limit));
  }
  if (ZEND_NUM_ARGS() == 2) {
    smart_str_appends(&_model_prepare_string, " OFFSET ");
    convert_to_string(_offset)
    smart_str_appends(&_model_prepare_string, Z_STRVAL_P(_offset));
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
  ZEND_PARSE_PARAMETERS_START(0, 0)
  ZEND_PARSE_PARAMETERS_END();
  zval array;
  array_init(&array);
  zend_update_property(
    model_handle,
    getThis(),
    "__origin__",
    sizeof("__origin__") - 1,
    &array TSRMLS_CC
  );
  zval_ptr_dtor(&array);
}

PHP_METHOD(Model, __set)
{
  zval *_name, *_value;
  ZEND_PARSE_PARAMETERS_START(2, 2)
    Z_PARAM_ZVAL(_name)
    Z_PARAM_ZVAL(_value)
  ZEND_PARSE_PARAMETERS_END();
  zval *rv;
  zval *_origin = zend_read_property(
    model_handle,
    getThis(),
    "__origin__",
    sizeof("__origin__") - 1,
    1,
    rv
  );
  zval value;
  ZVAL_COPY(&value, _value);
  if (!zend_symtable_exists_ind(Z_ARRVAL_P(_origin), Z_STR_P(_name))) {
    zend_hash_str_add_new(
      Z_ARRVAL_P(_origin),
      Z_STRVAL_P(_name),
      Z_STRLEN_P(_name),
      &value
    );
  }
  zend_update_property(
    model_handle,
    getThis(),
    Z_STRVAL_P(_name),
    Z_STRLEN_P(_name),
    &value
  );
}

PHP_METHOD(Model, update)
{
  zval *_columns = zend_read_static_property(
    zend_get_called_scope(execute_data),
    "__columns__",
    sizeof("__columns__") - 1,
    1
  );
  zval *rv;
  zval *_origin = zend_read_property(
    model_handle,
    getThis(),
    "__origin__",
    sizeof("__origin__") - 1,
    1,
    rv
  );
  zend_long columns_size = zend_hash_num_elements(Z_ARRVAL_P(_columns));
  smart_str update_string = {0};
  smart_str_appends(&update_string, "UPDATE ");
  zval *_table_name = zend_read_static_property(
    zend_get_called_scope(execute_data),
    "__table__",
    sizeof("__table__") - 1,
    1
  );
  smart_str_appends(&update_string, Z_STRVAL_P(_table_name));
  smart_str_appends(&update_string, " SET ");
  zval values;
  array_init(&values);
  zend_long values_size = 0;
  for (zend_long i = 0; i < columns_size; ++i) {
    Bucket *carry = Z_ARRVAL_P(_columns)->arData + i;
    zval *rvl;
    zval *_current = zend_read_property(
      model_handle,
      getThis(),
      Z_STRVAL_P(&carry->val),
      Z_STRLEN_P(&carry->val),
      1,
      rvl
    );
    zval *_raw = zend_hash_find(Z_ARRVAL_P(_origin), Z_STR_P(&carry->val));
    if (string_compare_function(_current, _raw) != 0) {
      if (values_size != 0) {
        smart_str_appends(&update_string, ", ");
      }
      ++values_size;
      smart_str_appends(&update_string, Z_STRVAL_P(&carry->val));
      smart_str_appends(&update_string, "=?");
      zend_hash_next_index_insert(Z_ARRVAL(values), _current);
      zend_hash_str_update(
        Z_ARRVAL_P(_origin),
        Z_STRVAL_P(&carry->val),
        Z_STRLEN_P(&carry->val),
        _current
      );
    }
  }
  smart_str_appends(&update_string, " WHERE ");
  zval *_primary_key = zend_read_static_property(
    zend_get_called_scope(execute_data),
    "__primary__",
    sizeof("__primary__") - 1,
    1
  );
  smart_str_appends(&update_string, Z_STRVAL_P(_primary_key));
  smart_str_appends(&update_string, " =?");
  smart_str_0(&update_string);
  zval *_primary_value = zend_hash_find(Z_ARRVAL_P(_origin), Z_STR_P(_primary_key));
  zval primary_value;
  ZVAL_COPY(&primary_value, _primary_value);
  zend_hash_next_index_insert(Z_ARRVAL(values), &primary_value);
  Z_ARRVAL(values)->nNumUsed = values_size + 1;
  Z_ARRVAL(values)->nNextFreeElement = values_size + 1;
  zend_hash_internal_pointer_reset(Z_ARRVAL(values));
  zval statement = vary_model_prepare(update_string.s);
  zval model_execute_name, model_execute_retval;
  ZVAL_STRING(&model_execute_name, "execute");
  zval args[1];
  ZVAL_COPY(&args[0], &values);
  call_user_function(
    EG(function_table),
    &statement,
    &model_execute_name,
    &model_execute_retval,
    1,
    args TSRMLS_CC
  );
  zval_ptr_dtor(&model_execute_name);
  zval_ptr_dtor(&model_execute_retval);
  zval_ptr_dtor(&args[0]);
  zval_ptr_dtor(&statement);
  zval_ptr_dtor(&values);
  RETURN_TRUE;
}

