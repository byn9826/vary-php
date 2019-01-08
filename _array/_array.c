#include "php.h"

zend_class_entry *_array_handle;

PHP_METHOD(_array, __construct)
{
  zval _items, *param;
  ZEND_PARSE_PARAMETERS_START(0, 1)
    Z_PARAM_OPTIONAL
    Z_PARAM_ARRAY(param)
  ZEND_PARSE_PARAMETERS_END();
  int num_args = ZEND_CALL_NUM_ARGS(execute_data);
  if (num_args == 0) {
    array_init(&_items);
  } else {
    if (num_args > 1) {
      warning_params_max_one("\\Vary\\Stack");
    }
    ZVAL_COPY(&_items, param);
  }
  zend_update_property(
    _array_handle,
    getThis(),
    "_items",
    sizeof("_items") - 1,
    &_items TSRMLS_CC
  );
  zval_ptr_dtor(&_items);
  RETURN_TRUE;
}

PHP_METHOD(_array, size)
{
  if (zend_parse_parameters_none() == FAILURE) {
    warning_params_exact_zero("\\Vary\\Stack size() method");
  }
  zval *rv, *_items;
  _items = zend_read_property(
    _array_handle,
    getThis(),
    "_items",
    sizeof("_items") - 1,
    0,
    rv TSRMLS_CC
  );
  RETURN_LONG(zend_hash_num_elements(Z_ARRVAL_P(_items)));
}

PHP_METHOD(_array, _addToBack)
{
  zval *_value, *rv, *_items, value;
  ZEND_PARSE_PARAMETERS_START(1, 1)
    Z_PARAM_ZVAL(_value)
  ZEND_PARSE_PARAMETERS_END();
  _items = zend_read_property(
    _array_handle,
    getThis(),
    "_items",
    sizeof("_items") - 1,
    0,
    rv TSRMLS_CC
  );
  ZVAL_COPY(&value, _value);
  uint32_t items_size = zend_hash_num_elements(Z_ARRVAL_P(_items));
  if (zend_hash_next_index_insert(Z_ARRVAL_P(_items), &value) == NULL) {
    zval_ptr_dtor(&value);
    RETURN_FALSE;
  }
  zend_update_property(
    _array_handle,
    getThis(),
    "_items",
    sizeof("_items") - 1,
    _items TSRMLS_CC
  );
  zval_ptr_dtor(&value);
  RETURN_TRUE;
}

PHP_METHOD(_array, _deleteFromBack)
{
  zval *_items, *rv;
  _items = zend_read_property(
    _array_handle,
    getThis(),
    "_items",
    sizeof("_items") - 1,
    0,
    rv TSRMLS_CC
  );
  uint32_t items_size = zend_hash_num_elements(Z_ARRVAL_P(_items));
  if (items_size == 0) {
		RETURN_NULL();
	}
  Bucket *last_item = Z_ARRVAL_P(_items)->arData + items_size - 1;
  zval return_item;
  ZVAL_COPY(&return_item, &last_item->val);
  zend_hash_del_bucket(Z_ARRVAL_P(_items), last_item);
  zend_hash_internal_pointer_reset(Z_ARRVAL_P(_items));
  RETURN_ZVAL(&return_item, 0, 0);
}
