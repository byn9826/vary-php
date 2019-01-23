#include "php.h"

zend_class_entry *_array_handle;
zend_class_entry *arrayList_handle;
zend_class_entry *stack_handle;
zend_class_entry *queue_handle;
zend_class_entry *deque_handle;

zval *array_getItems(zval *this) {
  zval *rv;
  return zend_read_property(
    arrayList_handle,
    this,
    "_items",
    sizeof("_items") - 1,
    0,
    rv TSRMLS_CC
  );
}

zval array_removeIndex(zval *this, zend_long target_index)
{
  zval removeIndex_name, removeIndex_retval;
  ZVAL_STRING(&removeIndex_name, "removeIndex");
  zval params[1];
  ZVAL_LONG(&params[0], target_index);
  call_user_function(
    EG(function_table),
    this,
    &removeIndex_name,
    &removeIndex_retval,
    1,
    params TSRMLS_CC
  );
  zval_ptr_dtor(&removeIndex_name);
  return removeIndex_retval;
}

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
  zval *_items = array_getItems(getThis());
  RETURN_LONG(zend_hash_num_elements(Z_ARRVAL_P(_items)));
}

PHP_METHOD(_array, unshift)
{
  zval *_value, value;
  ZEND_PARSE_PARAMETERS_START(1, 1)
    Z_PARAM_ZVAL(_value)
  ZEND_PARSE_PARAMETERS_END();
  zval *_items = array_getItems(getThis());
  uint32_t items_size = zend_hash_num_elements(Z_ARRVAL_P(_items));
  if (items_size == 0) {
    ZVAL_COPY(&value, _value);
    if (zend_hash_next_index_insert(Z_ARRVAL_P(_items), &value) == NULL) {
      zval_ptr_dtor(&value);
      RETURN_FALSE;
    }
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
    RETURN_TRUE;
  }
}

PHP_METHOD(_array, push)
{
  zval *_value, value;
  ZEND_PARSE_PARAMETERS_START(1, 1)
    Z_PARAM_ZVAL(_value)
  ZEND_PARSE_PARAMETERS_END();
  zval *_items = array_getItems(getThis());
  ZVAL_COPY(&value, _value);
  if (zend_hash_next_index_insert(Z_ARRVAL_P(_items), &value) == NULL) {
    zval_ptr_dtor(&value);
    RETURN_FALSE;
  }
  RETURN_TRUE;
}

PHP_METHOD(_array, removeIndex)
{
  zend_long target_index;
  ZEND_PARSE_PARAMETERS_START(1, 1)
    Z_PARAM_LONG(target_index)
  ZEND_PARSE_PARAMETERS_END();
  zval *_items = array_getItems(getThis());
  uint32_t items_size = zend_hash_num_elements(Z_ARRVAL_P(_items));
  if (items_size <= target_index) {
		RETURN_NULL();
	}
  Bucket *first_item = Z_ARRVAL_P(_items)->arData + target_index;
  zval return_item;
  ZVAL_COPY(&return_item, &first_item->val);
  zend_hash_del_bucket(Z_ARRVAL_P(_items), first_item);
  Bucket *carry;
  for (uint32_t i = target_index + 1; i < items_size; i++) {
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
  RETURN_ZVAL(&return_item, 1, 1);
}

PHP_METHOD(_array, shift)
{
  ZEND_PARSE_PARAMETERS_START(0, 0)
  ZEND_PARSE_PARAMETERS_END();
  zval *_items = array_getItems(getThis());
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
  RETURN_ZVAL(&return_item, 1, 1);
}

PHP_METHOD(_array, pop)
{
  ZEND_PARSE_PARAMETERS_START(0, 0)
  ZEND_PARSE_PARAMETERS_END();
  zval *_items = array_getItems(getThis());
  uint32_t items_size = zend_hash_num_elements(Z_ARRVAL_P(_items));
  if (items_size == 0) {
		RETURN_NULL();
	}
  Bucket *last_item = Z_ARRVAL_P(_items)->arData + items_size - 1;
  zval return_item;
  ZVAL_COPY(&return_item, &last_item->val);
  zend_hash_del_bucket(Z_ARRVAL_P(_items), last_item);
  Z_ARRVAL_P(_items)->nNumUsed = items_size - 1;
  Z_ARRVAL_P(_items)->nNextFreeElement = items_size - 1;
  zend_hash_internal_pointer_reset(Z_ARRVAL_P(_items));
  RETURN_ZVAL(&return_item, 1, 1);
}