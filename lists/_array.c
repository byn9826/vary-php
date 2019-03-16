#include "php.h"

zend_class_entry *_array_handle;
zend_class_entry *arrayList_handle;
zend_class_entry *stack_handle;
zend_class_entry *queue_handle;
zend_class_entry *deque_handle;
zend_class_entry *orderedList_handle;

zval *array_getValue(zval *this)
{
  zval *rv;
  return zend_read_property(
    arrayList_handle,
    this,
    "__value__",
    sizeof("__value__") - 1,
    0,
    rv TSRMLS_CC
  );
}

int vary_array_indexOf(zval *_items, zval *_value, int behavior)
{
  uint32_t items_size = zend_hash_num_elements(Z_ARRVAL_P(_items));
  int targetIndex = -1;
  for (int i = 0; i < items_size; i++) {
    int holder_index;
    Bucket *carry;
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
  return targetIndex;
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
    for (uint32_t i = target_index + 1; i < items_size; i++) {
      Bucket *carry = Z_ARRVAL_P(_items)->arData + i;
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

int vary_array_findIndex(
  zval *_items,
  zend_fcall_info user_func,
  zend_fcall_info_cache user_func_cache
)
{
  uint32_t full_size = zend_hash_num_elements(Z_ARRVAL_P(_items));
  uint32_t i = 0;
  for (i; i < full_size; ++i) {
    Bucket *carry = Z_ARRVAL_P(_items)->arData + i;
    zval args[1], retval;
    ZVAL_COPY_UNREF(&args[0], &carry->val);
    user_func.retval = &retval;
    user_func.param_count = 1;
    user_func.no_separation = 0;
    user_func.params = args;
    if (zend_call_function(&user_func, &user_func_cache) != SUCCESS) {
      zval_ptr_dtor(&args[0]);
      zval_ptr_dtor(&retval);
      return -1;
    }
    if (Z_TYPE(retval) == IS_TRUE) {
      zval_ptr_dtor(&args[0]);
      zval_ptr_dtor(&retval);
      break;
    }
    zval_ptr_dtor(&args[0]);
    zval_ptr_dtor(&retval);
  }
  if (i == full_size) {
    return -1;
  }
  return i;
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
    "__value__",
    sizeof("__value__") - 1,
    &_items TSRMLS_CC
  );
  zval_ptr_dtor(&_items);
  RETURN_TRUE;
}

PHP_METHOD(_array, value)
{
  ZEND_PARSE_PARAMETERS_START(0, 0)
  ZEND_PARSE_PARAMETERS_END();
  zval *_items = array_getValue(getThis());
  zval rtval;
  ZVAL_COPY(&rtval, _items);
  RETURN_ZVAL(&rtval, 1, 1);
}

PHP_METHOD(_array, length)
{
  ZEND_PARSE_PARAMETERS_START(0, 0)
  ZEND_PARSE_PARAMETERS_END();
  zval *_items = array_getValue(getThis());
  RETURN_LONG(zend_hash_num_elements(Z_ARRVAL_P(_items)));
}

PHP_METHOD(_array, unshift)
{
  zval *_value, value;
  ZEND_PARSE_PARAMETERS_START(1, 1)
    Z_PARAM_ZVAL(_value)
  ZEND_PARSE_PARAMETERS_END();
  zval *_items = array_getValue(getThis());
  uint32_t items_size = zend_hash_num_elements(Z_ARRVAL_P(_items));
  if (items_size == 0) {
    ZVAL_COPY(&value, _value);
    if (zend_hash_next_index_insert(Z_ARRVAL_P(_items), &value) == NULL) {
      zval_ptr_dtor(&value);
      RETURN_FALSE;
    }
    zval_ptr_dtor(&value);
    RETURN_TRUE;
  }
  Bucket *last_item = Z_ARRVAL_P(_items)->arData + items_size - 1;
  ZVAL_COPY(&value, &last_item->val);
  if (zend_hash_next_index_insert(Z_ARRVAL_P(_items), &value) == NULL) {
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

PHP_METHOD(_array, push)
{
  zval *_value, value;
  ZEND_PARSE_PARAMETERS_START(1, 1)
    Z_PARAM_ZVAL(_value)
  ZEND_PARSE_PARAMETERS_END();
  zval *_items = array_getValue(getThis());
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
  zval *_items = array_getValue(getThis());
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
  zval *_items = array_getValue(getThis());
  zval return_item = vary_array_removeIndex(_items, target_index);
  RETURN_ZVAL(&return_item, 1, 1);
}

PHP_METHOD(_array, shift)
{
  ZEND_PARSE_PARAMETERS_START(0, 0)
  ZEND_PARSE_PARAMETERS_END();
  zval *_items = array_getValue(getThis());
  uint32_t items_size = zend_hash_num_elements(Z_ARRVAL_P(_items));
  if (items_size == 0) {
		RETURN_NULL();
	}
  Bucket *first_item = Z_ARRVAL_P(_items)->arData;
  zval return_item;
  ZVAL_COPY(&return_item, &first_item->val);
  zend_hash_del_bucket(Z_ARRVAL_P(_items), first_item);
  for (uint32_t i = 1; i < items_size; i++) {
    Bucket *carry = Z_ARRVAL_P(_items)->arData + i;
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
  zval *_items = array_getValue(getThis());
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
  zval *_items = array_getValue(getThis());
  zend_fcall_info user_compare_func = empty_fcall_info;
	zend_fcall_info_cache user_compare_func_cache = empty_fcall_info_cache;
  ZEND_PARSE_PARAMETERS_START(0, 1)
    Z_PARAM_OPTIONAL
    Z_PARAM_FUNC(user_compare_func, user_compare_func_cache)
  ZEND_PARSE_PARAMETERS_END();
  vary_algorithm_shellSort(
    _items,
    user_compare_func,
    user_compare_func_cache,
    ZEND_NUM_ARGS() + 1
  );
}

PHP_METHOD(_array, reverse)
{
  ZEND_PARSE_PARAMETERS_START(0, 0)
  ZEND_PARSE_PARAMETERS_END();
  zval array;
  array_init(&array);
  zval *_items = array_getValue(getThis());
  int items_size = zend_hash_num_elements(Z_ARRVAL_P(_items));
  for (int i = items_size - 1; i >= 0; i--) {
    zval carry_value;
    Bucket *carry = Z_ARRVAL_P(_items)->arData + i;
    ZVAL_COPY_UNREF(&carry_value, &carry->val);
    if (zend_hash_next_index_insert(Z_ARRVAL(array), &carry_value) == NULL) {
      RETURN_NULL();
    }
    ZVAL_UNDEF(&carry_value);
    zval_ptr_dtor(&carry_value);
  }
  Z_ARRVAL(array)->nNumUsed = items_size;
  Z_ARRVAL(array)->nNextFreeElement = items_size;
  zend_hash_internal_pointer_reset(Z_ARRVAL(array));
  zend_update_property(
    _array_handle,
    getThis(),
    "__value__",
    sizeof("__value__") - 1,
    &array TSRMLS_CC
  );
  RETURN_ARR(Z_ARRVAL(array));
}

PHP_METHOD(_array, concat)
{
  zval *target;
  ZEND_PARSE_PARAMETERS_START(1, 1)
    Z_PARAM_ARRAY(target)
  ZEND_PARSE_PARAMETERS_END();
  zval *current = array_getValue(getThis());
  zval _items;
  array_init(&_items);
  uint32_t current_size = zend_hash_num_elements(Z_ARRVAL_P(current));
  for (uint32_t i = 0; i < current_size; i++) {
    zval carry_value;
    Bucket *carry = Z_ARRVAL_P(current)->arData + i;
    ZVAL_COPY_UNREF(&carry_value, &carry->val);
    if (zend_hash_next_index_insert(Z_ARRVAL(_items), &carry_value) == NULL) {
      RETURN_NULL();
    }
    ZVAL_UNDEF(&carry_value);
    zval_ptr_dtor(&carry_value);
  }
  uint32_t target_size = zend_hash_num_elements(Z_ARRVAL_P(target));
  for (uint32_t i = 0; i < target_size; i++) {
    zval carry_value;
    Bucket *carry = Z_ARRVAL_P(target)->arData + i;
    ZVAL_COPY_UNREF(&carry_value, &carry->val);
    if (zend_hash_next_index_insert(Z_ARRVAL(_items), &carry_value) == NULL) {
      RETURN_NULL();
    }
    ZVAL_UNDEF(&carry_value);
    zval_ptr_dtor(&carry_value);
  }
  Z_ARRVAL(_items)->nNumUsed = current_size + target_size;
  Z_ARRVAL(_items)->nNextFreeElement = current_size + target_size;
  zend_hash_internal_pointer_reset(Z_ARRVAL(_items));
  RETURN_ARR(Z_ARRVAL(_items));
}

PHP_METHOD(_array, indexOf)
{
  zval *_value;
  ZEND_PARSE_PARAMETERS_START(1, 1)
    Z_PARAM_ZVAL(_value)
  ZEND_PARSE_PARAMETERS_END();
  zval *_items = array_getValue(getThis());
  int result = vary_array_indexOf(_items, _value, 0);
  RETURN_LONG(result);
}

PHP_METHOD(_array, lastIndexOf)
{
  zval *_value;
  ZEND_PARSE_PARAMETERS_START(1, 1)
    Z_PARAM_ZVAL(_value)
  ZEND_PARSE_PARAMETERS_END();
  zval *_items = array_getValue(getThis());
  int result = vary_array_indexOf(_items, _value, 1);
  RETURN_LONG(result);
}

PHP_METHOD(_array, includes)
{
  zval *_value;
  ZEND_PARSE_PARAMETERS_START(1, 1)
    Z_PARAM_ZVAL(_value)
  ZEND_PARSE_PARAMETERS_END();
  zval *_items = array_getValue(getThis());
  int result = vary_array_indexOf(_items, _value, 1);
  if (result == -1) {
    RETURN_FALSE;
  }
  RETURN_TRUE;
}

PHP_METHOD(_array, every)
{
  zend_fcall_info user_compare_func = empty_fcall_info;
	zend_fcall_info_cache user_compare_func_cache = empty_fcall_info_cache;
  ZEND_PARSE_PARAMETERS_START(1, 1)
    Z_PARAM_FUNC(user_compare_func, user_compare_func_cache)
  ZEND_PARSE_PARAMETERS_END();
  zval *_items = array_getValue(getThis());
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
  zval *_items = array_getValue(getThis());
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

PHP_METHOD(_array, map)
{
  zend_fcall_info user_compare_func = empty_fcall_info;
	zend_fcall_info_cache user_compare_func_cache = empty_fcall_info_cache;
  ZEND_PARSE_PARAMETERS_START(1, 1)
    Z_PARAM_FUNC(user_compare_func, user_compare_func_cache)
  ZEND_PARSE_PARAMETERS_END();
  zval *_items = array_getValue(getThis());
  uint32_t full_size = zend_hash_num_elements(Z_ARRVAL_P(_items));
  zval _result;
  array_init(&_result);
  for (uint32_t i = 0; i < full_size; ++i) {
    Bucket *carry = Z_ARRVAL_P(_items)->arData + i;
    zval args[1], retval;
    ZVAL_COPY_UNREF(&args[0], &carry->val);
    user_compare_func.retval = &retval;
    user_compare_func.param_count = 1;
    user_compare_func.no_separation = 0;
    user_compare_func.params = args;
    if (zend_call_function(&user_compare_func, &user_compare_func_cache) != SUCCESS) {
      zval_ptr_dtor(&args[0]);
      zval_ptr_dtor(&retval);
      RETURN_NULL();
    }
    if (zend_hash_next_index_insert(Z_ARRVAL(_result), &retval) == NULL) {
      zval_ptr_dtor(&args[0]);
      zval_ptr_dtor(&retval);
      RETURN_NULL();
    }
    zval_ptr_dtor(&args[0]);
  }
  Z_ARRVAL(_result)->nNumUsed = full_size;
  Z_ARRVAL(_result)->nNextFreeElement = full_size;
  zend_hash_internal_pointer_reset(Z_ARRVAL(_result));
  RETURN_ARR(Z_ARRVAL(_result));
}

PHP_METHOD(_array, forEach)
{
  zend_fcall_info user_compare_func = empty_fcall_info;
	zend_fcall_info_cache user_compare_func_cache = empty_fcall_info_cache;
  ZEND_PARSE_PARAMETERS_START(1, 1)
    Z_PARAM_FUNC(user_compare_func, user_compare_func_cache)
  ZEND_PARSE_PARAMETERS_END();
  zval *_items = array_getValue(getThis());
  uint32_t full_size = zend_hash_num_elements(Z_ARRVAL_P(_items));
  for (uint32_t i = 0; i < full_size; ++i) {
    Bucket *carry = Z_ARRVAL_P(_items)->arData + i;
    zval args[1], retval;
    ZVAL_COPY_UNREF(&args[0], &carry->val);
    user_compare_func.retval = &retval;
    user_compare_func.param_count = 1;
    user_compare_func.no_separation = 0;
    user_compare_func.params = args;
    if (zend_call_function(&user_compare_func, &user_compare_func_cache) != SUCCESS) {
      zval_ptr_dtor(&args[0]);
      zval_ptr_dtor(&retval);
      RETURN_NULL();
    }
    zval_ptr_dtor(&args[0]);
    zval_ptr_dtor(&retval);
  }
  RETURN_NULL();
}

PHP_METHOD(_array, reduce)
{
  zend_fcall_info user_func = empty_fcall_info;
	zend_fcall_info_cache user_func_cache = empty_fcall_info_cache;
  zval sum_value, *start_value;
  ZEND_PARSE_PARAMETERS_START(1, 2)
    Z_PARAM_FUNC(user_func, user_func_cache)
    Z_PARAM_OPTIONAL
    Z_PARAM_ZVAL(start_value);
  ZEND_PARSE_PARAMETERS_END();
  zval *_items = array_getValue(getThis());
  int params_num = ZEND_NUM_ARGS();
  int start_index;
  if (params_num == 1) {
    Bucket *carry = Z_ARRVAL_P(_items)->arData;
    ZVAL_COPY_UNREF(&sum_value, &carry->val);
    start_index = 1;
  } else {
    ZVAL_COPY_UNREF(&sum_value, start_value);
    start_index = 0;
  }
  for (uint32_t i = start_index; i < zend_hash_num_elements(Z_ARRVAL_P(_items)); ++i)
  {
    Bucket *carry = Z_ARRVAL_P(_items)->arData + i;
    zval args[2];
    ZVAL_COPY_UNREF(&args[0], &sum_value);
    zval_ptr_dtor(&sum_value);
    ZVAL_COPY(&args[1], &carry->val);
    user_func.retval = &sum_value;
    user_func.param_count = 2;
    user_func.no_separation = 0;
    user_func.params = args;
    if (zend_call_function(&user_func, &user_func_cache) != SUCCESS) {
      zval_ptr_dtor(&args[0]);
      zval_ptr_dtor(&args[1]);
      zval_ptr_dtor(&sum_value);
      RETURN_NULL();
    }
    zval_ptr_dtor(&args[0]);
    zval_ptr_dtor(&args[1]);
  }
  RETURN_ZVAL(&sum_value, 0, 1);
}

PHP_METHOD(_array, findIndex)
{
  zend_fcall_info user_func = empty_fcall_info;
	zend_fcall_info_cache user_func_cache = empty_fcall_info_cache;
  ZEND_PARSE_PARAMETERS_START(1, 1)
    Z_PARAM_FUNC(user_func, user_func_cache)
  ZEND_PARSE_PARAMETERS_END();
  zval *_items = array_getValue(getThis());
  int index = vary_array_findIndex(_items, user_func, user_func_cache);
  RETURN_LONG(index);
}

PHP_METHOD(_array, find)
{
  zend_fcall_info user_func = empty_fcall_info;
	zend_fcall_info_cache user_func_cache = empty_fcall_info_cache;
  ZEND_PARSE_PARAMETERS_START(1, 1)
    Z_PARAM_FUNC(user_func, user_func_cache)
  ZEND_PARSE_PARAMETERS_END();
  zval *_items = array_getValue(getThis());
  int index = vary_array_findIndex(_items, user_func, user_func_cache);
  if (index == -1) {
    RETURN_NULL();
  }
  Bucket *target = Z_ARRVAL_P(_items)->arData + index;
  zval rtval;
  ZVAL_COPY(&rtval, &target->val);
  RETURN_ZVAL(&rtval, 0, 0);
}

PHP_METHOD(_array, filter)
{
  zend_fcall_info user_compare_func = empty_fcall_info;
	zend_fcall_info_cache user_compare_func_cache = empty_fcall_info_cache;
  ZEND_PARSE_PARAMETERS_START(1, 1)
    Z_PARAM_FUNC(user_compare_func, user_compare_func_cache)
  ZEND_PARSE_PARAMETERS_END();
  zval *_items = array_getValue(getThis());
  uint32_t full_size = zend_hash_num_elements(Z_ARRVAL_P(_items));
  uint32_t filter_size = 0;
  zval _result;
  array_init(&_result);
  for (uint32_t i = 0; i < full_size; ++i) {
    Bucket *carry = Z_ARRVAL_P(_items)->arData + i;
    zval args[1], retval;
    ZVAL_COPY_UNREF(&args[0], &carry->val);
    user_compare_func.retval = &retval;
    user_compare_func.param_count = 1;
    user_compare_func.no_separation = 0;
    user_compare_func.params = args;
    if (zend_call_function(&user_compare_func, &user_compare_func_cache) != SUCCESS) {
      zval_ptr_dtor(&args[0]);
      zval_ptr_dtor(&retval);
      RETURN_NULL();
    }
    if (Z_TYPE(retval) == IS_TRUE) {
      if (zend_hash_next_index_insert(Z_ARRVAL(_result), &args[0]) == NULL) {
        zval_ptr_dtor(&args[0]);
        zval_ptr_dtor(&retval);
        RETURN_NULL();
      }
      ZVAL_UNDEF(&args[0]);
      ++filter_size;
    }
    zval_ptr_dtor(&args[0]);
    zval_ptr_dtor(&retval);
  }
  Z_ARRVAL(_result)->nNumUsed = filter_size;
  Z_ARRVAL(_result)->nNextFreeElement = filter_size;
  zend_hash_internal_pointer_reset(Z_ARRVAL(_result));
  RETURN_ARR(Z_ARRVAL(_result));
}

PHP_METHOD(OrderedList, __construct)
{
  ZEND_PARSE_PARAMETERS_START(0, 0)
  ZEND_PARSE_PARAMETERS_END();
  zval _items;
  array_init(&_items);
  zend_update_property(
    _array_handle,
    getThis(),
    "__value__",
    sizeof("__value__") - 1,
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
  zval *_items = array_getValue(getThis());
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
  zval *_items = array_getValue(getThis());
  zval _binarySearch_retval = vary_algorithm_binarySearch(_items, target_value);
  Bucket *carry = Z_ARRVAL_P(&_binarySearch_retval)->arData + 1;
  if (zval_get_long(&carry->val) == 0) {
    zval_ptr_dtor(&_binarySearch_retval);
    RETURN_FALSE;
  }
  carry = Z_ARRVAL_P(&_binarySearch_retval)->arData;
  zend_long target_index = zval_get_long(&carry->val);
  zval_ptr_dtor(&_binarySearch_retval);
  zval return_item = vary_array_removeIndex(array_getValue(getThis()), target_index);
  zval_ptr_dtor(&return_item);
  RETURN_LONG(target_index);
}

PHP_METHOD(OrderedList, indexOf)
{
  zend_long target_value;
  ZEND_PARSE_PARAMETERS_START(1, 1)
    Z_PARAM_LONG(target_value)
  ZEND_PARSE_PARAMETERS_END();
  zval *_items = array_getValue(getThis());
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
