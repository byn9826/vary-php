#include "php.h"

zend_class_entry *orderedList_handle;

PHP_METHOD(OrderedList, __construct)
{
  zval _items, *param;
  ZEND_PARSE_PARAMETERS_START(0, 1)
    Z_PARAM_OPTIONAL
    Z_PARAM_ARRAY(param)
  ZEND_PARSE_PARAMETERS_END();
  if (ZEND_NUM_ARGS() == 0) {
    array_init(&_items);
  } else {
    ZVAL_COPY(&_items, param);
    vary_algorithm_shellSort(&_items, empty_fcall_info, empty_fcall_info_cache, 1);
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

PHP_METHOD(OrderedList, add)
{
  zend_long new_value;
  ZEND_PARSE_PARAMETERS_START(1, 1)
    Z_PARAM_LONG(new_value)
  ZEND_PARSE_PARAMETERS_END();
  zval *_items = array_getItems(getThis());
  zval _binarySearch_retval = vary_algorithm_binarySearch(_items, new_value);
  Bucket *carry = Z_ARRVAL_P(&_binarySearch_retval)->arData + 1;
  if (zval_get_long(&carry->val) == 1) {
    zval_ptr_dtor(&_binarySearch_retval);
    RETURN_FALSE;
  }
  carry = Z_ARRVAL_P(&_binarySearch_retval)->arData;
  zend_long target_index = zval_get_long(&carry->val);
  zval_ptr_dtor(&_binarySearch_retval);
  uint32_t items_size = zend_hash_num_elements(Z_ARRVAL_P(_items));
  zval value;
  if (target_index == items_size) {
    ZVAL_LONG(&value, new_value);
    if (zend_hash_next_index_insert(Z_ARRVAL_P(_items), &value) == NULL) {
      zval_ptr_dtor(&value);
      RETURN_FALSE;
    }
  } else {
    carry = Z_ARRVAL_P(_items)->arData + items_size - 1;
    ZVAL_COPY(&value, &carry->val);
    if (zend_hash_next_index_insert(Z_ARRVAL_P(_items), &value) == NULL) {
      zval_ptr_dtor(&value);
      RETURN_FALSE;
    }
    if (items_size >= 2) {
      for (int i = items_size - 2; i >= target_index; i--) {
        carry = Z_ARRVAL_P(_items)->arData + i;
        Bucket *holder = Z_ARRVAL_P(_items)->arData + i + 1;
        holder->h = i + 1;
        holder->key = NULL;
        ZVAL_COPY_VALUE(&holder->val, &carry->val);
        ZVAL_UNDEF(&carry->val);
      }
    }
    carry = Z_ARRVAL_P(_items)->arData + target_index;
    ZVAL_LONG(&value, new_value);
    ZVAL_COPY_VALUE(&carry->val, &value);
  }
  Z_ARRVAL_P(_items)->nNumUsed = items_size + 1;
  Z_ARRVAL_P(_items)->nNextFreeElement = items_size + 1;
  zend_hash_internal_pointer_reset(Z_ARRVAL_P(_items));
  zval_ptr_dtor(&value);
  RETURN_LONG(target_index);
}

PHP_METHOD(OrderedList, remove)
{
  zend_long target_value;
  ZEND_PARSE_PARAMETERS_START(1, 1)
    Z_PARAM_LONG(target_value)
  ZEND_PARSE_PARAMETERS_END();
  zval *_items = array_getItems(getThis());
  zval _binarySearch_retval = vary_algorithm_binarySearch(_items, target_value);
  Bucket *carry = Z_ARRVAL_P(&_binarySearch_retval)->arData + 1;
  if (zval_get_long(&carry->val) == 0) {
    zval_ptr_dtor(&_binarySearch_retval);
    RETURN_FALSE;
  }
  carry = Z_ARRVAL_P(&_binarySearch_retval)->arData;
  zend_long target_index = zval_get_long(&carry->val);
  zval_ptr_dtor(&_binarySearch_retval);
  zval return_item = vary_array_removeIndex(array_getItems(getThis()), target_index);
  zval_ptr_dtor(&return_item);
  RETURN_LONG(target_index);
}

PHP_METHOD(OrderedList, indexOf)
{
  zend_long target_value;
  ZEND_PARSE_PARAMETERS_START(1, 1)
    Z_PARAM_LONG(target_value)
  ZEND_PARSE_PARAMETERS_END();
  zval *_items = array_getItems(getThis());
  zval _binarySearch_retval = vary_algorithm_binarySearch(_items, target_value);
  Bucket *carry = Z_ARRVAL_P(&_binarySearch_retval)->arData + 1;
  if (zval_get_long(&carry->val) == 0) {
    zval_ptr_dtor(&_binarySearch_retval);
    RETURN_FALSE;
  }
  carry = Z_ARRVAL_P(&_binarySearch_retval)->arData;
  zend_long target_index = zval_get_long(&carry->val);
  zval_ptr_dtor(&_binarySearch_retval);
  RETURN_LONG(target_index);
}
