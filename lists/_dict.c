#include "php.h"

zend_class_entry *_dict_handle;
zend_class_entry *mapList_handle;

zval *vary_dict_getValue(zval *this)
{
  return vary_list_getValue(_dict_handle, this);
}

void vary_dict_setValue(zval *this, zval value)
{
  vary_list_setValue(_dict_handle, this, value);
}


PHP_METHOD(_dict, __construct)
{
  vary_list_init_normal(INTERNAL_FUNCTION_PARAM_PASSTHRU, 1);
}

PHP_METHOD(_dict, value)
{
  ZEND_PARSE_PARAMETERS_START(0, 0)
  ZEND_PARSE_PARAMETERS_END();
  zval *_dict = vary_dict_getValue(getThis());
  zval dict;
  ZVAL_COPY(&dict, _dict);
  RETURN_ARR(Z_ARRVAL(dict));
}

PHP_METHOD(_dict, size)
{
  vary_list_length(INTERNAL_FUNCTION_PARAM_PASSTHRU, 1);
}

PHP_METHOD(_dict, setValue)
{
  zval dict, *_dict;
  ZEND_PARSE_PARAMETERS_START(1, 1)
    Z_PARAM_ARRAY(_dict)
  ZEND_PARSE_PARAMETERS_END();
  ZVAL_COPY(&dict, _dict);
  vary_array_setValue(getThis(), dict);
  zval_ptr_dtor(&dict);
  RETURN_TRUE;
}

PHP_METHOD(_dict, clear)
{
  vary_list_init_empty(INTERNAL_FUNCTION_PARAM_PASSTHRU, 1);
}

PHP_METHOD(_dict, has)
{
  zval *_key;
  ZEND_PARSE_PARAMETERS_START(1, 1)
    Z_PARAM_ZVAL(_key)
  ZEND_PARSE_PARAMETERS_END();
  zval *_dict = vary_dict_getValue(getThis());
  zend_bool is_exist = 0;
  switch (Z_TYPE_P(_key)) {
		case IS_STRING:
			if (zend_symtable_exists_ind(Z_ARRVAL_P(_dict), Z_STR_P(_key))) {
				is_exist = 1;
			}
      break;
		case IS_LONG:
			if (zend_hash_index_exists(Z_ARRVAL_P(_dict), Z_LVAL_P(_key))) {
				is_exist = 1;
			}
			break;
		default:
      break;
	}
  if (is_exist == 0) {
    RETURN_FALSE;
  } else {
    RETURN_TRUE;
  }
}

PHP_METHOD(_dict, set)
{
  zval *_key, *_value;
  ZEND_PARSE_PARAMETERS_START(2, 2)
    Z_PARAM_ZVAL(_key)
    Z_PARAM_ZVAL(_value)
  ZEND_PARSE_PARAMETERS_END();
  zval *_dict = vary_dict_getValue(getThis());
  switch (Z_TYPE_P(_key)) {
    zval value;
		case IS_STRING:
      ZVAL_COPY(&value, _value);
      if (zend_symtable_exists_ind(Z_ARRVAL_P(_dict), Z_STR_P(_key))) {
				zend_hash_str_update(Z_ARRVAL_P(_dict), Z_STRVAL_P(_key), Z_STRLEN_P(_key), &value);
			} else {
        zend_hash_str_add_new(Z_ARRVAL_P(_dict), Z_STRVAL_P(_key), Z_STRLEN_P(_key), &value);
      }
      break;
		case IS_LONG:
      ZVAL_COPY(&value, _value);
			if (zend_hash_index_exists(Z_ARRVAL_P(_dict), Z_LVAL_P(_key))) {
				zend_hash_index_update(Z_ARRVAL_P(_dict), Z_LVAL_P(_key), &value);
			} else {
        zend_hash_index_add_new(Z_ARRVAL_P(_dict), Z_LVAL_P(_key), &value);
      }
			break;
		default:
			RETURN_FALSE;
      break;
	}
  RETURN_TRUE;
}

PHP_METHOD(_dict, delete)
{
  zval *_key;
  ZEND_PARSE_PARAMETERS_START(1, 1)
    Z_PARAM_ZVAL(_key)
  ZEND_PARSE_PARAMETERS_END();
  zval *_dict = vary_dict_getValue(getThis());
  switch (Z_TYPE_P(_key)) {
		case IS_STRING:
      zend_hash_del_ind(Z_ARRVAL_P(_dict), Z_STR_P(_key));
			break;
		case IS_LONG:
      zend_hash_index_del(Z_ARRVAL_P(_dict), Z_LVAL_P(_key));
			break;
		default:
			RETURN_TRUE;
	}
  RETURN_TRUE;
}

PHP_METHOD(_dict, get)
{
  zval *_key, *_value;
  ZEND_PARSE_PARAMETERS_START(1, 1)
    Z_PARAM_ZVAL(_key)
  ZEND_PARSE_PARAMETERS_END();
  zval *_dict = vary_dict_getValue(getThis());
  switch (Z_TYPE_P(_key)) {
		case IS_STRING:
      _value = zend_hash_find(Z_ARRVAL_P(_dict), Z_STR_P(_key));
			break;
		case IS_LONG:
      _value = zend_hash_index_find(Z_ARRVAL_P(_dict), Z_LVAL_P(_key));
			break;
		default:
			RETURN_NULL();
	}
  if (!_value) {
    RETURN_NULL();
  }
  RETURN_ZVAL(_value, 1, 0);
}
