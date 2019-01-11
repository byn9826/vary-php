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
  ZEND_PARSE_PARAMETERS_START(0, 0)
  ZEND_PARSE_PARAMETERS_END();
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

PHP_METHOD(_array, _addToFront)
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
  uint32_t items_size = zend_hash_num_elements(Z_ARRVAL_P(_items));
  if (items_size == 0) {
    ZVAL_COPY(&value, _value);
    if (zend_hash_next_index_insert(Z_ARRVAL_P(_items), &value) == NULL) {
      zval_ptr_dtor(&value);
      RETURN_FALSE;
    }
    zval_ptr_dtor(&value);
    RETURN_TRUE;
  } else {
    Bucket *last_item = Z_ARRVAL_P(_items)->arData + items_size - 1;
    ZVAL_COPY(&value, &last_item->val);
    if (zend_hash_next_index_insert(Z_ARRVAL_P(_items), &value) == NULL) {
      zval_ptr_dtor(&value);
      RETURN_FALSE;
    }
    if (items_size >= 2) {
      for (int i = items_size - 2; i >= 0; i--) {
        last_item = Z_ARRVAL_P(_items)->arData + i;
        Bucket *holder = Z_ARRVAL_P(_items)->arData + i + 1;
        holder->h = i + 1;
        holder->key = NULL;
        ZVAL_COPY_VALUE(&holder->val, &last_item->val);
        ZVAL_UNDEF(&last_item->val);
      }
    }
    last_item = Z_ARRVAL_P(_items)->arData;
    ZVAL_COPY(&value, _value);
    ZVAL_COPY_VALUE(&last_item->val, &value);
    Z_ARRVAL_P(_items)->nNumUsed = items_size + 1;
    Z_ARRVAL_P(_items)->nNextFreeElement = items_size + 1;
    zend_hash_internal_pointer_reset(Z_ARRVAL_P(_items));
    zval_ptr_dtor(&value);
    RETURN_TRUE;
  }
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
  if (zend_hash_next_index_insert(Z_ARRVAL_P(_items), &value) == NULL) {
    zval_ptr_dtor(&value);
    RETURN_FALSE;
  }
  zval_ptr_dtor(&value);
  RETURN_TRUE;
}

PHP_METHOD(_array, _deleteFromFront)
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
  Bucket *first_item = Z_ARRVAL_P(_items)->arData;
  zval return_item;
  ZVAL_COPY(&return_item, &first_item->val);
  zend_hash_del_bucket(Z_ARRVAL_P(_items), first_item);
  Bucket *carry;
  for (uint32_t i = 1; i < items_size; i++) {
    carry = Z_ARRVAL_P(_items)->arData + i;
    Bucket *holder = Z_ARRVAL_P(_items)->arData + i - 1;
    holder->h = i - 1;
    holder->key = NULL;
    ZVAL_COPY_VALUE(&holder->val, &carry->val);
    ZVAL_UNDEF(&carry->val);
  }
  Z_ARRVAL_P(_items)->nNumUsed = items_size - 1;
  Z_ARRVAL_P(_items)->nNextFreeElement = items_size - 1;
  zend_hash_internal_pointer_reset(Z_ARRVAL_P(_items));
  RETURN_ZVAL(&return_item, 0, 0);
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

zval _array_addToFront(zval *this, zval *_value) {
  zval _addToFront_name, _addToFront_retval;
  ZVAL_STRING(&_addToFront_name, "_addToFront");
  zval params[1];
  ZVAL_COPY(&params[0], _value);
  call_user_function(
    EG(function_table),
    this,
    &_addToFront_name,
    &_addToFront_retval,
    1,
    params TSRMLS_CC
  );
  zval_ptr_dtor(&_addToFront_name);
  return _addToFront_retval;
}

zval _array_addToBack(zval *this, zval *_value) {
  zval _addToBack_name, _addToBack_retval;
  ZVAL_STRING(&_addToBack_name, "_addToBack");
  zval params[1];
  ZVAL_COPY(&params[0], _value);
  call_user_function(
    EG(function_table),
    this,
    &_addToBack_name,
    &_addToBack_retval,
    1,
    params TSRMLS_CC
  );
  zval_ptr_dtor(&_addToBack_name);
  return _addToBack_retval;
}

zval _array__deleteFromFront(zval *this) {
  zval _deleteFromFront_name, _deleteFromFront_retval;
  ZVAL_STRING(&_deleteFromFront_name, "_deleteFromFront");
  call_user_function(
    EG(function_table),
    this,
    &_deleteFromFront_name,
    &_deleteFromFront_retval,
    0,
    NULL TSRMLS_CC
  );
  zval_ptr_dtor(&_deleteFromFront_name);
  return _deleteFromFront_retval;
}

zval _array_deleteFromBack(zval *this) {
  zval _deleteFromBack_name, _deleteFromBack_retval;
  ZVAL_STRING(&_deleteFromBack_name, "_deleteFromBack");
  call_user_function(
    EG(function_table),
    this,
    &_deleteFromBack_name,
    &_deleteFromBack_retval,
    0,
    NULL TSRMLS_CC
  );
  zval_ptr_dtor(&_deleteFromBack_name);
  return _deleteFromBack_retval;
}