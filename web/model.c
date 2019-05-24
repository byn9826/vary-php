#include "php.h"
#include "ext/pdo/php_pdo_driver.h"

zend_class_entry *model_handle;

static void vary_model_callHooks(zval *this, zend_long type)
{
  zval model_hook_name, model_hook_retval;
  switch (type)
  {
  case 0:
    ZVAL_STRING(&model_hook_name, "beforeCreate");
    break;
  case 1:
    ZVAL_STRING(&model_hook_name, "afterCreate");
    break;
  case 2:
    ZVAL_STRING(&model_hook_name, "beforeUpdate");
    break;
  case 3:
    ZVAL_STRING(&model_hook_name, "afterUpdate");
    break;
  default:
    return;
  }
  
  call_user_function(
    EG(function_table),
    this,
    &model_hook_name,
    &model_hook_retval,
    0,
    NULL TSRMLS_CC
  );
  zval_ptr_dtor(&model_hook_name);
  zval_ptr_dtor(&model_hook_retval);
}

static void vary_model_callConfig(zend_string *_name)
{
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
}

static zval *vary_model_getOrigin(zval *this)
{
  zval *rv;
  return zend_read_property(
    model_handle,
    this,
    "__origin__",
    sizeof("__origin__") - 1,
    1,
    rv
  );
} 

static zval *vary_model_getTableName(zend_execute_data *execute_data)
{
  return zend_read_static_property(
    zend_get_called_scope(execute_data),
    "__table__",
    sizeof("__table__") - 1,
    1
  );
}

static zval *vary_model_getPrimaryKey(zend_execute_data *execute_data)
{
  return zend_read_static_property(
    zend_get_called_scope(execute_data),
    "__primary__",
    sizeof("__primary__") - 1,
    1
  );
}

static void vary_model_setFetchMode(zval statement, zend_string *_name, zend_long mode)
{
  zval model_mode_name, model_mode_retval;
  ZVAL_STRING(&model_mode_name, "setFetchMode");
  zval args[2];
  ZVAL_LONG(&args[0], mode);
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
}

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

static zval vary_model_execute(zval statement, zend_long params_num, zval params_val[])
{
  zval model_execute_name, model_execute_retval;
  ZVAL_STRING(&model_execute_name, "execute");
  call_user_function(
    EG(function_table),
    &statement,
    &model_execute_name,
    &model_execute_retval,
    params_num,
    params_val TSRMLS_CC
  );
  zval_ptr_dtor(&model_execute_name);
  zval_ptr_dtor(&model_execute_retval);
}

static zval vary_model_fetch(zval statement, zend_long type)
{
  zval model_fetch_name, model_fetch_retval;
  if (type == 1) {
    ZVAL_STRING(&model_fetch_name, "fetch");
  } else {
    ZVAL_STRING(&model_fetch_name, "fetchAll");
  }
  if (type == 0 || type == 1) {
    call_user_function(
      EG(function_table),
      &statement,
      &model_fetch_name,
      &model_fetch_retval,
      0,
      NULL TSRMLS_CC
    );
  } else {
    zval args[1];
    ZVAL_LONG(&args[0], PDO_FETCH_COLUMN);
    call_user_function(
      EG(function_table),
      &statement,
      &model_fetch_name,
      &model_fetch_retval,
      1,
      args TSRMLS_CC
    );
    zval_ptr_dtor(&args[0]);
  }
  zval_ptr_dtor(&statement);
  zval_ptr_dtor(&model_fetch_name);
  return model_fetch_retval;
}

static smart_str vary_model_buildSelect(zval *_list, zend_execute_data *execute_data, zend_long mode)
{
  smart_str _model_prepare_string = {0};
  smart_str_appends(&_model_prepare_string, "SELECT ");
  if (mode == 1) {
    zval select_key, *select_value;
    ZVAL_STRING(&select_key, "select");
    select_value = zend_hash_find(Z_ARRVAL_P(_list), Z_STR(select_key));
    if (select_value) {
      zend_long select_size = zend_hash_num_elements(Z_ARRVAL_P(select_value));
      for (zend_long i = 0; i < select_size; ++i) {
        Bucket *select = Z_ARRVAL_P(select_value)->arData + i;
        smart_str_appends(&_model_prepare_string, Z_STRVAL_P(&select->val));
        smart_str_appends(&_model_prepare_string, ", ");
      }
      zval *_primary_key = vary_model_getPrimaryKey(execute_data);
      smart_str_appends(&_model_prepare_string, Z_STRVAL_P(_primary_key));
    } else {
      smart_str_appends(&_model_prepare_string, "*");
    }
    zval_ptr_dtor(&select_key);
  } else {
    smart_str_appends(&_model_prepare_string, "*");
  }
  smart_str_appends(&_model_prepare_string, " FROM ");
  zval *_table_name = vary_model_getTableName(execute_data);
  smart_str_appends(&_model_prepare_string, Z_STRVAL_P(_table_name));
  return _model_prepare_string;
}

static zend_long vary_model_buildWhere(zval *_list, smart_str _model_prepare_string, zval where_array)
{
  zend_long where_size = 0;
  zval where_key, *where_value;
  ZVAL_STRING(&where_key, "where");
  where_value = zend_hash_find(Z_ARRVAL_P(_list), Z_STR(where_key));
  zval_ptr_dtor(&where_key);
  if (where_value) {
    smart_str_appends(&_model_prepare_string, " WHERE ");
    zval *zv;
    zend_long num_key;
    zend_string *str_key;
    smart_str_appends(&_model_prepare_string, " ( ");
    ZEND_HASH_FOREACH_KEY_VAL_IND(Z_ARRVAL_P(where_value), num_key, str_key, zv) {
      if (str_key) {
        if (where_size != 0) {
          smart_str_appends(&_model_prepare_string, " AND ");
        }
        smart_str_appends(&_model_prepare_string, ZSTR_VAL(str_key));
        smart_str_appends(&_model_prepare_string, "=?");
        zval *_str_value = zend_hash_find(Z_ARRVAL_P(where_value), str_key);
        zval str_value;
        ZVAL_COPY(&str_value, _str_value);
        zend_hash_next_index_insert(
          Z_ARRVAL(where_array),
          &str_value
        );
        ++where_size;
      }
    } ZEND_HASH_FOREACH_END();
    smart_str_appends(&_model_prepare_string, " ) ");
    zval orWhere_key, *orWhere_value;
    ZVAL_STRING(&orWhere_key, "orWhere");
    orWhere_value = zend_hash_find(Z_ARRVAL_P(_list), Z_STR(orWhere_key));
    zval_ptr_dtor(&orWhere_key);
    if (orWhere_value) {
      smart_str_appends(&_model_prepare_string, "OR ( ");
      zend_long orWhere_size = 0;
      ZEND_HASH_FOREACH_KEY_VAL_IND(Z_ARRVAL_P(where_value), num_key, str_key, zv) {
        if (str_key) {
          if (orWhere_size != 0) {
            smart_str_appends(&_model_prepare_string, " AND ");
          }
          smart_str_appends(&_model_prepare_string, ZSTR_VAL(str_key));
          smart_str_appends(&_model_prepare_string, "=?");
          zval *_str_value = zend_hash_find(Z_ARRVAL_P(orWhere_value), str_key);
          zval str_value;
          ZVAL_COPY(&str_value, _str_value);
          zend_hash_next_index_insert(
            Z_ARRVAL(where_array),
            &str_value
          );
          ++orWhere_size;
        }
      } ZEND_HASH_FOREACH_END();
      where_size += orWhere_size;
      smart_str_appends(&_model_prepare_string, " ) ");
    }
    Z_ARRVAL(where_array)->nNumUsed = where_size;
    Z_ARRVAL(where_array)->nNextFreeElement = where_size;
    zend_hash_internal_pointer_reset(Z_ARRVAL(where_array));
  }
  return where_size;
}

static void vary_model_buildOrderBy(zval *_list, smart_str _model_prepare_string)
{
  zval order_key, *order_value;
  ZVAL_STRING(&order_key, "orderBy");
  order_value = zend_hash_find(Z_ARRVAL_P(_list), Z_STR(order_key));
  if (order_value) {
    smart_str_appends(&_model_prepare_string, " ORDER BY ");
    zend_long order_size = zend_hash_num_elements(Z_ARRVAL_P(order_value));
    for (zend_long i = 0; i < order_size; ++i) {
      Bucket *order = Z_ARRVAL_P(order_value)->arData + i;
      smart_str_appends(&_model_prepare_string, Z_STRVAL_P(&order->val));
      if (i != order_size - 1) {
        smart_str_appends(&_model_prepare_string, ",");
      }
    }
  }
  zval_ptr_dtor(&order_key);
}

static void vary_model_buildLimitOffset(zval *_list, smart_str _model_prepare_string)
{
  zval limit_key, *limit_value;
  ZVAL_STRING(&limit_key, "limit");
  limit_value = zend_hash_find(Z_ARRVAL_P(_list), Z_STR(limit_key));
  if (limit_value) {
    smart_str_appends(&_model_prepare_string, " LIMIT ");
    convert_to_string(limit_value)
    smart_str_appends(&_model_prepare_string, Z_STRVAL_P(limit_value));
  }
  zval_ptr_dtor(&limit_key);
  zval offset_key, *offset_value;
  ZVAL_STRING(&offset_key, "offset");
  offset_value = zend_hash_find(Z_ARRVAL_P(_list), Z_STR(offset_key));
  if (offset_value) {
    smart_str_appends(&_model_prepare_string, " OFFSET ");
    convert_to_string(offset_value)
    smart_str_appends(&_model_prepare_string, Z_STRVAL_P(offset_value));
  }
  zval_ptr_dtor(&offset_key);
  smart_str_0(&_model_prepare_string);
}

PHP_METHOD(Model, config) {}

PHP_METHOD(Model, useTable)
{
  zend_string *_name;
  ZEND_PARSE_PARAMETERS_START(1, 1)
    Z_PARAM_STR(_name)
  ZEND_PARSE_PARAMETERS_END();
  zval *_table_name = vary_model_getTableName(execute_data);
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
    vary_model_execute(statement, 0, NULL);
    zval columns = vary_model_fetch(statement, 2);
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

PHP_METHOD(Model, get)
{
  zval *_param;
  ZEND_PARSE_PARAMETERS_START(0, 1)
    Z_PARAM_OPTIONAL
    Z_PARAM_ZVAL(_param);
  ZEND_PARSE_PARAMETERS_END();
  zend_string *_name = zend_get_called_scope(execute_data)->name;
  vary_model_callConfig(_name);
  zval *_table_name = vary_model_getTableName(execute_data);
  zval values;
  array_init(&values);
  zend_long mode = Z_TYPE_P(_param) == IS_ARRAY ? 1 : 0;
  smart_str _model_prepare_string = vary_model_buildSelect(_param, execute_data, mode);
  switch (Z_TYPE_P(_param)) {
    case IS_STRING:
    case IS_LONG: {
      smart_str_appends(&_model_prepare_string, " WHERE ");
      zval *_primary_key = vary_model_getPrimaryKey(execute_data);
      smart_str_appends(&_model_prepare_string, Z_STRVAL_P(_primary_key));
      smart_str_appends(&_model_prepare_string, " =?");
      smart_str_0(&_model_prepare_string);
      zend_hash_next_index_insert(Z_ARRVAL(values), _param);
      Z_ARRVAL(values)->nNumUsed = 1;
      Z_ARRVAL(values)->nNextFreeElement = 1;
      zend_hash_internal_pointer_reset(Z_ARRVAL(values));
			break;
    }
		case IS_ARRAY: {
      vary_model_buildWhere(_param, _model_prepare_string, values);
      vary_model_buildOrderBy(_param, _model_prepare_string);
      vary_model_buildLimitOffset(_param, _model_prepare_string);
			break;
    }
		default:
			RETURN_FALSE;
  }
  zval statement = vary_model_prepare(_model_prepare_string.s);
  vary_model_setFetchMode(
    statement,
    _name,
    PDO_FETCH_CLASS | PDO_FETCH_PROPS_LATE
  );
  vary_model_execute(statement, 1, &values);
  zval_ptr_dtor(&values);
  zval result = vary_model_fetch(statement, 1);
  RETURN_ZVAL(&result, 0, 1);
}

PHP_METHOD(Model, list)
{
  zval *_list;
  ZEND_PARSE_PARAMETERS_START(0, 1)
    Z_PARAM_OPTIONAL
    Z_PARAM_ARRAY(_list)
  ZEND_PARSE_PARAMETERS_END();
  zend_string *_name = zend_get_called_scope(execute_data)->name;
  vary_model_callConfig(_name);
  smart_str _model_prepare_string = vary_model_buildSelect(
    _list,
    execute_data,
    ZEND_NUM_ARGS()
  );
  zval where_array;
  array_init(&where_array);
  zend_long where_size = 0;
  if (ZEND_NUM_ARGS() == 1) {
    where_size = vary_model_buildWhere(_list, _model_prepare_string, where_array);
    vary_model_buildOrderBy(_list, _model_prepare_string);
    vary_model_buildLimitOffset(_list, _model_prepare_string);
  }
  zval statement = vary_model_prepare(_model_prepare_string.s);
  vary_model_setFetchMode(
    statement,
    _name,
    PDO_FETCH_CLASS | PDO_FETCH_PROPS_LATE
  );
  if (where_size != 0) {
    vary_model_execute(statement, 1, &where_array);
  } else {
    vary_model_execute(statement, 0, NULL);
  }
  zval_ptr_dtor(&where_array);
  zval result = vary_model_fetch(statement, 0);
  RETURN_ARR(Z_ARRVAL(result));
}

PHP_METHOD(Model, updating)
{
  zval *_list;
  ZEND_PARSE_PARAMETERS_START(1, 1)
    Z_PARAM_ARRAY(_list)
  ZEND_PARSE_PARAMETERS_END();
  zend_string *_name = zend_get_called_scope(execute_data)->name;
  vary_model_callConfig(_name);
  smart_str _model_prepare_string = {0};
  smart_str_appends(&_model_prepare_string, "Update ");
  zval *_table_name = vary_model_getTableName(execute_data);
  smart_str_appends(&_model_prepare_string, Z_STRVAL_P(_table_name));
  smart_str_appends(&_model_prepare_string, " SET ");
  zval set_key, *set_value;
  ZVAL_STRING(&set_key, "set");
  set_value = zend_hash_find(Z_ARRVAL_P(_list), Z_STR(set_key));
  zval_ptr_dtor(&set_key);
  zval *_primary_key = vary_model_getPrimaryKey(execute_data);
  zval values;
  array_init(&values);
  zend_long values_size = 0;
  zval *zv;
  zend_long num_key;
  zend_string *str_key;
  ZEND_HASH_FOREACH_KEY_VAL_IND(Z_ARRVAL_P(set_value), num_key, str_key, zv) {
    if (str_key) {
      if (zend_string_equals(str_key, Z_STR_P(_primary_key)) == 0) {
        if (values_size != 0) {
          smart_str_appends(&_model_prepare_string, ", ");
        }
        smart_str_appends(&_model_prepare_string, ZSTR_VAL(str_key));
        smart_str_appends(&_model_prepare_string, "=?");
        zval *_str_value = zend_hash_find(Z_ARRVAL_P(set_value), str_key);
        zval str_value;
        ZVAL_COPY(&str_value, _str_value);
        zend_hash_next_index_insert(
          Z_ARRVAL(values),
          &str_value
        );
        ++values_size;
      }
    }
  } ZEND_HASH_FOREACH_END();
  zend_long where_size = vary_model_buildWhere(_list, _model_prepare_string, values);
  values_size += where_size;
  Z_ARRVAL(values)->nNumUsed = values_size;
  Z_ARRVAL(values)->nNextFreeElement = values_size;
  zend_hash_internal_pointer_reset(Z_ARRVAL(values));
  smart_str_0(&_model_prepare_string);
  zval statement = vary_model_prepare(_model_prepare_string.s);
  vary_model_execute(statement, 1, &values);
  zval_ptr_dtor(&statement);
  zval_ptr_dtor(&values);
  RETURN_TRUE;
}

PHP_METHOD(Model, deleting)
{
  zval *_list;
  ZEND_PARSE_PARAMETERS_START(1, 1)
    Z_PARAM_ARRAY(_list)
  ZEND_PARSE_PARAMETERS_END();
  zend_string *_name = zend_get_called_scope(execute_data)->name;
  vary_model_callConfig(_name);
  smart_str _model_prepare_string = {0};
  smart_str_appends(&_model_prepare_string, "DELETE FROM ");
  zval *_table_name = vary_model_getTableName(execute_data);
  smart_str_appends(&_model_prepare_string, Z_STRVAL_P(_table_name));
  zval values;
  array_init(&values);
  zend_long values_size = vary_model_buildWhere(_list, _model_prepare_string, values);
  Z_ARRVAL(values)->nNumUsed = values_size;
  Z_ARRVAL(values)->nNextFreeElement = values_size;
  zend_hash_internal_pointer_reset(Z_ARRVAL(values));
  smart_str_0(&_model_prepare_string);
  zval statement = vary_model_prepare(_model_prepare_string.s);
  vary_model_execute(statement, 1, &values);
  zval_ptr_dtor(&statement);
  zval_ptr_dtor(&values);
  RETURN_TRUE;
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
  zend_string *_name = zend_get_called_scope(execute_data)->name;
  vary_model_callConfig(_name);
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
  zval *_origin = vary_model_getOrigin(getThis());
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

PHP_METHOD(Model, delete)
{
  smart_str delete_string = {0};
  smart_str_appends(&delete_string, "DELETE FROM ");
  zval *_table_name = vary_model_getTableName(execute_data);
  smart_str_appends(&delete_string, Z_STRVAL_P(_table_name));
  smart_str_appends(&delete_string, " WHERE ");
  zval *_primary_key = vary_model_getPrimaryKey(execute_data);
  smart_str_appends(&delete_string, Z_STRVAL_P(_primary_key));
  smart_str_appends(&delete_string, " =?");
  smart_str_0(&delete_string);
  zval values;
  array_init(&values);
  zval *_origin = vary_model_getOrigin(getThis());
  zval *_primary_value = zend_hash_find(Z_ARRVAL_P(_origin), Z_STR_P(_primary_key));
  zval primary_value;
  ZVAL_COPY(&primary_value, _primary_value);
  zend_hash_next_index_insert(Z_ARRVAL(values), &primary_value);
  Z_ARRVAL(values)->nNumUsed = 1;
  Z_ARRVAL(values)->nNextFreeElement = 1;
  zend_hash_internal_pointer_reset(Z_ARRVAL(values));
  zval statement = vary_model_prepare(delete_string.s);
  vary_model_execute(statement, 1, &values);
  zval_ptr_dtor(&statement);
  zval_ptr_dtor(&values);
  RETURN_TRUE;
}

PHP_METHOD(Model, create)
{
  vary_model_callHooks(getThis(), 0);
  smart_str create_string = {0};
  smart_str_appends(&create_string, "INSERT INTO ");
  zval *_table_name = vary_model_getTableName(execute_data);
  smart_str_appends(&create_string, Z_STRVAL_P(_table_name));
  smart_str_appends(&create_string, " (");
  zval *_columns = zend_read_static_property(
    zend_get_called_scope(execute_data),
    "__columns__",
    sizeof("__columns__") - 1,
    1
  );
  zend_long columns_size = zend_hash_num_elements(Z_ARRVAL_P(_columns));
  zval *_primary_key = vary_model_getPrimaryKey(execute_data);
  zval *_origin = vary_model_getOrigin(getThis());
  zval values;
  array_init(&values);
  zend_long values_size = 0;
  for (zend_long i = 0; i < columns_size; ++i) {
    Bucket *carry = Z_ARRVAL_P(_columns)->arData + i;
    if (string_compare_function(&carry->val, _primary_key) != 0) {
      zval *rv;
      zval *_current = zend_read_property(
        model_handle,
        getThis(),
        Z_STRVAL_P(&carry->val),
        Z_STRLEN_P(&carry->val),
        1,
        rv
      );
      if (values_size != 0) {
        smart_str_appends(&create_string, ", ");
      }
      ++values_size;
      smart_str_appends(&create_string, Z_STRVAL_P(&carry->val));
      zend_hash_next_index_insert(Z_ARRVAL(values), _current);
      zend_hash_str_update(
        Z_ARRVAL_P(_origin),
        Z_STRVAL_P(&carry->val),
        Z_STRLEN_P(&carry->val),
        _current
      );
    }
  }
  smart_str_appends(&create_string, ") VALUES (");
  for (zend_long j = 0; j < values_size; ++j) {
    smart_str_appends(&create_string, "?");
    if (j != values_size - 1) {
      smart_str_appends(&create_string, ", ");
    }
  }
  smart_str_appends(&create_string, ")");
  smart_str_0(&create_string);
  Z_ARRVAL(values)->nNumUsed = values_size;
  Z_ARRVAL(values)->nNextFreeElement = values_size;
  zend_hash_internal_pointer_reset(Z_ARRVAL(values));
  zval statement = vary_model_prepare(create_string.s);
  vary_model_execute(statement, 1, &values);
  zval_ptr_dtor(&statement);
  zval_ptr_dtor(&values);
  zval conn = vary_conn_getConnection();
  zval conn_inserted_name, conn_inserted_retval;
  ZVAL_STRING(&conn_inserted_name, "lastInsertId");
  call_user_function(
    EG(function_table),
    &conn,
    &conn_inserted_name,
    &conn_inserted_retval,
    0,
    NULL TSRMLS_CC
  );
  zval_ptr_dtor(&conn);
  zval_ptr_dtor(&conn_inserted_name);
  zend_update_property(
    model_handle,
    getThis(),
    Z_STRVAL_P(_primary_key),
    Z_STRLEN_P(_primary_key),
    &conn_inserted_retval TSRMLS_CC
  );
  zend_hash_str_add_new(
    Z_ARRVAL_P(_origin),
    Z_STRVAL_P(_primary_key),
    Z_STRLEN_P(_primary_key),
    &conn_inserted_retval
  );
  vary_model_callHooks(getThis(), 1);
  RETURN_TRUE;
}

PHP_METHOD(Model, update)
{
  vary_model_callHooks(getThis(), 2);
  zval *_columns = zend_read_static_property(
    zend_get_called_scope(execute_data),
    "__columns__",
    sizeof("__columns__") - 1,
    1
  );
  zval *rv;
  zval *_origin = vary_model_getOrigin(getThis());
  zend_long columns_size = zend_hash_num_elements(Z_ARRVAL_P(_columns));
  smart_str update_string = {0};
  smart_str_appends(&update_string, "UPDATE ");
  zval *_table_name = vary_model_getTableName(execute_data);
  smart_str_appends(&update_string, Z_STRVAL_P(_table_name));
  smart_str_appends(&update_string, " SET ");
  zval values;
  array_init(&values);
  zend_long values_size = 0;
  zval *_primary_key = vary_model_getPrimaryKey(execute_data);
  for (zend_long i = 0; i < columns_size; ++i) {
    Bucket *carry = Z_ARRVAL_P(_columns)->arData + i;
    if (string_compare_function(&carry->val, _primary_key) != 0) {
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
  }
  smart_str_appends(&update_string, " WHERE ");
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
  vary_model_execute(statement, 1, &values);
  zval_ptr_dtor(&statement);
  zval_ptr_dtor(&values);
  vary_model_callHooks(getThis(), 3);
  RETURN_TRUE;
}

PHP_METHOD(Model, beforeCreate) {}

PHP_METHOD(Model, afterCreate) {}

PHP_METHOD(Model, beforeUpdate) {}

PHP_METHOD(Model, afterUpdate) {}