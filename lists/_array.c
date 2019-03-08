#include "php.h"

zend_class_entry *_array_handle;
zend_class_entry *arrayList_handle;
zend_class_entry *stack_handle;
zend_class_entry *queue_handle;
zend_class_entry *deque_handle;

zval *array_getItems(zval *this)
{
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

void vary_array_indexOf(INTERNAL_FUNCTION_PARAMETERS, int behavior)
{
  zval *_value;
  ZEND_PARSE_PARAMETERS_START(1, 1)
    Z_PARAM_ZVAL(_value)
  ZEND_PARSE_PARAMETERS_END();
  zval *_items = array_getItems(getThis());
  uint32_t items_size = zend_hash_num_elements(Z_ARRVAL_P(_items));
  int targetIndex = -1;
  Bucket *carry;
  for (int i = 0; i < items_size; i++) {
    int holder_index;
    if (behavior == 0) {
      holder_index = i;
      carry = Z_ARRVAL_P(_items)->arData + holder_index;
    } else {
      holder_index = items_size - 1 - i;
      carry = Z_ARRVAL_P(_items)->arData + holder_index;
    }
    if (fast_is_identical_function(_value, &carry->val) == 1) {
      targetIndex = holder_index;
      break;
    }
  }
  RETURN_LONG(targetIndex);
}

zval vary_array_removeIndex(zval *_items, zend_long target_index)
{
  zval return_item;
  ZVAL_NULL(&return_item);
  uint32_t items_size = zend_hash_num_elements(Z_ARRVAL_P(_items));
  if (items_size > target_index) {
    Bucket *first_item = Z_ARRVAL_P(_items)->arData + target_index;
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
	}
  return return_item;
}

PHP_METHOD(_array, __construct)
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

PHP_METHOD(_array, value)
{
  ZEND_PARSE_PARAMETERS_START(0, 0)
  ZEND_PARSE_PARAMETERS_END();
  zval *_items = array_getItems(getThis());
  zval rtval;
  ZVAL_COPY(&rtval, _items);
  RETURN_ZVAL(&rtval, 1, 1);
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

PHP_METHOD(_array, index)
{
  zend_long target_index;
  ZEND_PARSE_PARAMETERS_START(1, 1)
    Z_PARAM_LONG(target_index)
  ZEND_PARSE_PARAMETERS_END();
  zval *_items = array_getItems(getThis());
  uint32_t items_size = zend_hash_num_elements(Z_ARRVAL_P(_items));
  if (target_index + 1 > items_size || target_index < 0) {
    RETURN_NULL();
  }
  zval return_item;
  Bucket *target_item = Z_ARRVAL_P(_items)->arData + target_index;
  ZVAL_COPY(&return_item, &target_item->val);
  RETURN_ZVAL(&return_item, 1, 1);
}

PHP_METHOD(_array, removeIndex)
{
  zend_long target_index;
  ZEND_PARSE_PARAMETERS_START(1, 1)
    Z_PARAM_LONG(target_index)
  ZEND_PARSE_PARAMETERS_END();
  zval *_items = array_getItems(getThis());
  zval return_item = vary_array_removeIndex(_items, target_index);
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

PHP_METHOD(_array, sort)
{
  zval *_items = array_getItems(getThis());
  zend_fcall_info user_compare_func = empty_fcall_info;
	zend_fcall_info_cache user_compare_func_cache = empty_fcall_info_cache;
  ZEND_PARSE_PARAMETERS_START(0, 1)
    Z_PARAM_OPTIONAL
    Z_PARAM_FUNC(user_compare_func, user_compare_func_cache)
  ZEND_PARSE_PARAMETERS_END();
  vary_algorithm_shellSort(_items, user_compare_func, user_compare_func_cache, ZEND_NUM_ARGS() + 1);
}

PHP_METHOD(_array, concat)
{
  zval *target;
  ZEND_PARSE_PARAMETERS_START(1, 1)
    Z_PARAM_ARRAY(target)
  ZEND_PARSE_PARAMETERS_END();
  zval *current = array_getItems(getThis());
  zval _items;
  array_init(&_items);
  uint32_t current_size = zend_hash_num_elements(Z_ARRVAL_P(current));
  Bucket *carry;
  zval carry_value;
  for (uint32_t i = 0; i < current_size; i++) {
    carry = Z_ARRVAL_P(current)->arData + i;
    ZVAL_COPY(&carry_value, &carry->val);
    if (zend_hash_next_index_insert(Z_ARRVAL_P(&_items), &carry_value) == NULL) {
      RETURN_NULL();
    }
  }
  uint32_t target_size = zend_hash_num_elements(Z_ARRVAL_P(target));
  for (uint32_t i = 0; i < target_size; i++) {
    carry = Z_ARRVAL_P(target)->arData + i;
    ZVAL_COPY(&carry_value, &carry->val);
    if (zend_hash_next_index_insert(Z_ARRVAL_P(&_items), &carry_value) == NULL) {
      RETURN_NULL();
    }
  }
  zval_ptr_dtor(&carry_value);
  Z_ARRVAL_P(&_items)->nNumUsed = current_size + target_size;
  Z_ARRVAL_P(&_items)->nNextFreeElement = current_size + target_size;
  zend_hash_internal_pointer_reset(Z_ARRVAL_P(&_items));
  RETURN_ARR(Z_ARRVAL_P(&_items));
}

PHP_METHOD(_array, indexOf)
{
  vary_array_indexOf(INTERNAL_FUNCTION_PARAM_PASSTHRU, 0);
}

PHP_METHOD(_array, lastIndexOf)
{
  vary_array_indexOf(INTERNAL_FUNCTION_PARAM_PASSTHRU, 1);
}

PHP_METHOD(_array, every)
{
  zend_fcall_info user_compare_func = empty_fcall_info;
	zend_fcall_info_cache user_compare_func_cache = empty_fcall_info_cache;
  ZEND_PARSE_PARAMETERS_START(1, 1)
    Z_PARAM_FUNC(user_compare_func, user_compare_func_cache)
  ZEND_PARSE_PARAMETERS_END();
  zval *_items = array_getItems(getThis());
  uint32_t full_size = zend_hash_num_elements(Z_ARRVAL_P(_items));
  int result = 1;
  for (uint32_t i = 0; i < full_size; ++i) {
    Bucket *current_item = Z_ARRVAL_P(_items)->arData + i;
    zval retval;
    zval args[1];
    ZVAL_COPY(&args[0], &current_item->val);
    user_compare_func.retval = &retval;
    user_compare_func.param_count = 1;
    user_compare_func.no_separation = 0;
    user_compare_func.params = args;
    if (
      zend_call_function(&user_compare_func, &user_compare_func_cache) != SUCCESS
      || Z_TYPE(retval) == IS_FALSE
    ) {
      zval_ptr_dtor(&args[0]);
      zval_ptr_dtor(&retval);
      result = 0;
      break;
    }
    zval_ptr_dtor(&args[0]);
    zval_ptr_dtor(&retval);
  }
  if (result == 0) {
    RETURN_FALSE;
  } else {
    RETURN_TRUE;
  }
}

PHP_METHOD(_array, some)
{
  zend_fcall_info user_compare_func = empty_fcall_info;
	zend_fcall_info_cache user_compare_func_cache = empty_fcall_info_cache;
  ZEND_PARSE_PARAMETERS_START(1, 1)
    Z_PARAM_FUNC(user_compare_func, user_compare_func_cache)
  ZEND_PARSE_PARAMETERS_END();
  zval *_items = array_getItems(getThis());
  uint32_t full_size = zend_hash_num_elements(Z_ARRVAL_P(_items));
  int result = 0;
  for (uint32_t i = 0; i < full_size; ++i) {
    Bucket *current_item = Z_ARRVAL_P(_items)->arData + i;
    zval retval;
    zval args[1];
    ZVAL_COPY(&args[0], &current_item->val);
    user_compare_func.retval = &retval;
    user_compare_func.param_count = 1;
    user_compare_func.no_separation = 0;
    user_compare_func.params = args;
    if (
      zend_call_function(&user_compare_func, &user_compare_func_cache) == SUCCESS
      && Z_TYPE(retval) == IS_TRUE
    ) {
      zval_ptr_dtor(&args[0]);
      zval_ptr_dtor(&retval);
      result = 1;
      break;
    }
    zval_ptr_dtor(&args[0]);
    zval_ptr_dtor(&retval);
  }
  if (result == 0) {
    RETURN_FALSE;
  } else {
    RETURN_TRUE;
  }
}