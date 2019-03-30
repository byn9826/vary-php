#include "php.h"

zend_class_entry *_array_handle;
zend_class_entry *arrayList_handle;
zend_class_entry *setList_handle;
zend_class_entry *stack_handle;
zend_class_entry *queue_handle;
zend_class_entry *deque_handle;
zend_class_entry *orderedList_handle;

zval *vary_array_getValue(zval *this)
{
  return vary_lists_getValue(_array_handle, this);
}

void vary_array_setValue(zval *this, zval value)
{
  vary_lists_setValue(_array_handle, this, value);
}

zend_long vary_array_indexOf(zval *_array, zval *_value, zend_long behavior)
{
  zend_long array_size = zend_hash_num_elements(Z_ARRVAL_P(_array));
  zend_long targetIndex = -1;
  for (zend_long i = 0; i < array_size; i++) {
    zend_long holder_index;
    Bucket *carry;
    if (behavior == 0) {
      holder_index = i;
      carry = Z_ARRVAL_P(_array)->arData + holder_index;
    } else {
      holder_index = array_size - 1 - i;
      carry = Z_ARRVAL_P(_array)->arData + holder_index;
    }
    if (fast_is_identical_function(_value, &carry->val) == 1) {
      targetIndex = holder_index;
      break;
    }
  }
  return targetIndex;
}

zval vary_array_removeIndex(zval *_array, zend_long target_index)
{
  zval return_item;
  ZVAL_NULL(&return_item);
  zend_long array_size = zend_hash_num_elements(Z_ARRVAL_P(_array));
  if (array_size > target_index) {
    Bucket *first_item = Z_ARRVAL_P(_array)->arData + target_index;
    ZVAL_COPY(&return_item, &first_item->val);
    zend_hash_del_bucket(Z_ARRVAL_P(_array), first_item);
    for (zend_long i = target_index + 1; i < array_size; i++) {
      Bucket *carry = Z_ARRVAL_P(_array)->arData + i;
      Bucket *holder = Z_ARRVAL_P(_array)->arData + i - 1;
      ZVAL_COPY_VALUE(&holder->val, &carry->val);
    }
    Z_ARRVAL_P(_array)->nNumUsed = array_size - 1;
    Z_ARRVAL_P(_array)->nNextFreeElement = array_size - 1;
    zend_hash_internal_pointer_reset(Z_ARRVAL_P(_array));
  }
  return return_item;
}

zend_long vary_array_findIndex(
  zval *_array,
  zend_fcall_info user_func,
  zend_fcall_info_cache user_func_cache
)
{
  zend_long array_size = zend_hash_num_elements(Z_ARRVAL_P(_array));
  zend_long i = 0;
  for (i; i < array_size; ++i) {
    Bucket *carry = Z_ARRVAL_P(_array)->arData + i;
    zval args[1], retval;
    ZVAL_COPY(&args[0], &carry->val);
    user_func.retval = &retval;
    user_func.param_count = 1;
    user_func.no_separation = 0;
    user_func.params = args;
    zend_call_function(&user_func, &user_func_cache);
    zval_ptr_dtor(&args[0]);
    if (Z_TYPE(retval) == IS_TRUE) {
      zval_ptr_dtor(&retval);
      break;
    }
    zval_ptr_dtor(&retval);
  }
  if (i == array_size) {
    return -1;
  }
  return i;
}

zval vary_array_reduce(
  zval *_array,
  zend_fcall_info user_func,
  zend_fcall_info_cache user_func_cache,
  zend_long params_num,
  zval *start_value,
  zend_long behavior
) {
  zend_long start_index;
  zend_long array_size = zend_hash_num_elements(Z_ARRVAL_P(_array));
  zval sum_value;
  Bucket *carry;
  if (params_num == 1) {
    if (behavior == 0) {
      carry = Z_ARRVAL_P(_array)->arData;
    } else {
      carry = Z_ARRVAL_P(_array)->arData + array_size - 1;
    }
    ZVAL_COPY(&sum_value, &carry->val);
    start_index = 1;
  } else {
    ZVAL_COPY(&sum_value, start_value);
    start_index = 0;
  }
  for (zend_long i = start_index; i < array_size; ++i)
  {
    if (behavior == 0) {
      carry = Z_ARRVAL_P(_array)->arData + i;
    } else {
      carry = Z_ARRVAL_P(_array)->arData + array_size - 1 - i;
    }
    zval args[2];
    ZVAL_COPY(&args[0], &sum_value);
    zval_ptr_dtor(&sum_value);
    ZVAL_COPY(&args[1], &carry->val);
    user_func.retval = &sum_value;
    user_func.param_count = 2;
    user_func.no_separation = 0;
    user_func.params = args;
    zend_call_function(&user_func, &user_func_cache);
    zval_ptr_dtor(&args[0]);
    zval_ptr_dtor(&args[1]);
  }
  return sum_value;
}

PHP_METHOD(_array, __construct)
{
  zval array, *_array;
  ZEND_PARSE_PARAMETERS_START(0, 1)
    Z_PARAM_OPTIONAL
    Z_PARAM_ARRAY(_array)
  ZEND_PARSE_PARAMETERS_END();
  if (ZEND_NUM_ARGS() == 0) {
    array_init(&array);
  } else {
    ZVAL_COPY(&array, _array);
  }
  vary_array_setValue(getThis(), array);
  zval_ptr_dtor(&array);
  RETURN_TRUE;
}

PHP_METHOD(_array, setValue)
{
  zval array, *_array;
  ZEND_PARSE_PARAMETERS_START(1, 1)
    Z_PARAM_ARRAY(_array)
  ZEND_PARSE_PARAMETERS_END();
  ZVAL_COPY(&array, _array);
  vary_array_setValue(getThis(), array);
  zval_ptr_dtor(&array);
  RETURN_TRUE;
}

PHP_METHOD(_array, value)
{
  ZEND_PARSE_PARAMETERS_START(0, 0)
  ZEND_PARSE_PARAMETERS_END();
  zval *_array = vary_array_getValue(getThis());
  zval array;
  ZVAL_COPY(&array, _array);
  RETURN_ARR(Z_ARRVAL(array));
}

PHP_METHOD(_array, length)
{
  ZEND_PARSE_PARAMETERS_START(0, 0)
  ZEND_PARSE_PARAMETERS_END();
  zval *_array = vary_array_getValue(getThis());
  RETURN_LONG(zend_hash_num_elements(Z_ARRVAL_P(_array)));
}

PHP_METHOD(_array, unshift)
{
  zval *_value, value;
  ZEND_PARSE_PARAMETERS_START(1, 1)
    Z_PARAM_ZVAL(_value)
  ZEND_PARSE_PARAMETERS_END();
  zval *_array = vary_array_getValue(getThis());
  zend_long array_size = zend_hash_num_elements(Z_ARRVAL_P(_array));
  if (array_size == 0) {
    ZVAL_COPY(&value, _value);
    zend_hash_next_index_insert(Z_ARRVAL_P(_array), &value);
    RETURN_TRUE;
  }
  Bucket *carry = Z_ARRVAL_P(_array)->arData + array_size - 1;
  zval carry_value;
  ZVAL_COPY(&carry_value, &carry->val);
  zend_hash_next_index_insert(Z_ARRVAL_P(_array), &carry_value);
  zval_ptr_dtor(&carry_value);
  if (array_size >= 2) {
    for (zend_long i = array_size - 2; i >= 0; --i) {
      carry = Z_ARRVAL_P(_array)->arData + i;
      Bucket *holder = Z_ARRVAL_P(_array)->arData + i + 1;
      ZVAL_COPY_VALUE(&holder->val, &carry->val);
    }
  }
  ZVAL_COPY(&value, _value);
  carry = Z_ARRVAL_P(_array)->arData;
  ZVAL_COPY_VALUE(&carry->val, &value);
  RETURN_TRUE;
}

PHP_METHOD(_array, push)
{
  zval *_value, value;
  ZEND_PARSE_PARAMETERS_START(1, 1)
    Z_PARAM_ZVAL(_value)
  ZEND_PARSE_PARAMETERS_END();
  zval *_array = vary_array_getValue(getThis());
  ZVAL_COPY(&value, _value);
  zend_hash_next_index_insert(Z_ARRVAL_P(_array), &value);
  RETURN_TRUE;
}

PHP_METHOD(_array, index)
{
  zend_long target_index;
  ZEND_PARSE_PARAMETERS_START(1, 1)
    Z_PARAM_LONG(target_index)
  ZEND_PARSE_PARAMETERS_END();
  zval *_array = vary_array_getValue(getThis());
  zend_long array_size = zend_hash_num_elements(Z_ARRVAL_P(_array));
  if (target_index + 1 > array_size || target_index < 0) {
    RETURN_NULL();
  }
  Bucket *target_item = Z_ARRVAL_P(_array)->arData + target_index;
  RETURN_ZVAL(&target_item->val, 1, 0);
}

PHP_METHOD(_array, removeIndex)
{
  zend_long target_index;
  ZEND_PARSE_PARAMETERS_START(1, 1)
    Z_PARAM_LONG(target_index)
  ZEND_PARSE_PARAMETERS_END();
  zval *_array = vary_array_getValue(getThis());
  zval return_item = vary_array_removeIndex(_array, target_index);
  RETURN_ZVAL(&return_item, 0, 1);
}

PHP_METHOD(_array, shift)
{
  ZEND_PARSE_PARAMETERS_START(0, 0)
  ZEND_PARSE_PARAMETERS_END();
  zval *_array = vary_array_getValue(getThis());
  zend_long array_size = zend_hash_num_elements(Z_ARRVAL_P(_array));
  if (array_size == 0) {
    RETURN_NULL();
  }
  Bucket *first_item = Z_ARRVAL_P(_array)->arData;
  zval return_item;
  ZVAL_COPY(&return_item, &first_item->val);
  zend_hash_del_bucket(Z_ARRVAL_P(_array), first_item);
  for (zend_long i = 1; i < array_size; i++) {
    Bucket *carry = Z_ARRVAL_P(_array)->arData + i;
    Bucket *holder = Z_ARRVAL_P(_array)->arData + i - 1;
    ZVAL_COPY_VALUE(&holder->val, &carry->val);
    ZVAL_UNDEF(&carry->val);
  }
  Z_ARRVAL_P(_array)->nNumUsed = array_size - 1;
  Z_ARRVAL_P(_array)->nNextFreeElement = array_size - 1;
  zend_hash_internal_pointer_reset(Z_ARRVAL_P(_array));
  RETURN_ZVAL(&return_item, 0, 1);
}

PHP_METHOD(_array, pop)
{
  ZEND_PARSE_PARAMETERS_START(0, 0)
  ZEND_PARSE_PARAMETERS_END();
  zval *_array = vary_array_getValue(getThis());
  zend_long array_size = zend_hash_num_elements(Z_ARRVAL_P(_array));
  if (array_size == 0) {
    RETURN_NULL();
  }
  Bucket *last_item = Z_ARRVAL_P(_array)->arData + array_size - 1;
  zval return_item;
  ZVAL_COPY(&return_item, &last_item->val);
  zend_hash_del_bucket(Z_ARRVAL_P(_array), last_item);
  Z_ARRVAL_P(_array)->nNumUsed = array_size - 1;
  Z_ARRVAL_P(_array)->nNextFreeElement = array_size - 1;
  zend_hash_internal_pointer_reset(Z_ARRVAL_P(_array));
  RETURN_ZVAL(&return_item, 0, 1);
}

PHP_METHOD(_array, sort)
{
  zval *_array = vary_array_getValue(getThis());
  zend_fcall_info user_func = empty_fcall_info;
  zend_fcall_info_cache user_func_cache = empty_fcall_info_cache;
  ZEND_PARSE_PARAMETERS_START(0, 1)
    Z_PARAM_OPTIONAL
    Z_PARAM_FUNC(user_func, user_func_cache)
  ZEND_PARSE_PARAMETERS_END();
  vary_algorithm_shellSort(
    _array,
    user_func,
    user_func_cache,
    ZEND_NUM_ARGS() + 1
  );
}

PHP_METHOD(_array, reverse)
{
  ZEND_PARSE_PARAMETERS_START(0, 0)
  ZEND_PARSE_PARAMETERS_END();
  zval array;
  array_init(&array);
  zval *_array = vary_array_getValue(getThis());
  zend_long array_size = zend_hash_num_elements(Z_ARRVAL_P(_array));
  for (zend_long i = array_size - 1; i >= 0; --i) {
    Bucket *carry = Z_ARRVAL_P(_array)->arData + i;
    zend_hash_next_index_insert(Z_ARRVAL(array), &carry->val);
    ZVAL_UNDEF(&carry->val);
  }
  Z_ARRVAL(array)->nNumUsed = array_size;
  Z_ARRVAL(array)->nNextFreeElement = array_size;
  zend_hash_internal_pointer_reset(Z_ARRVAL(array));
  vary_array_setValue(getThis(), array);
  zval_ptr_dtor(&array);
  RETURN_TRUE;
}

PHP_METHOD(_array, concat)
{
  zval *target;
  ZEND_PARSE_PARAMETERS_START(1, 1)
    Z_PARAM_ARRAY(target)
  ZEND_PARSE_PARAMETERS_END();
  zval *current = vary_array_getValue(getThis());
  zval array;
  array_init(&array);
  zval value;
  zend_long current_size = zend_hash_num_elements(Z_ARRVAL_P(current));
  for (zend_long i = 0; i < current_size; i++) {
    Bucket *carry = Z_ARRVAL_P(current)->arData + i;
    ZVAL_COPY(&value, &carry->val);
    zend_hash_next_index_insert(Z_ARRVAL(array), &value);
  }
  zend_long target_size = zend_hash_num_elements(Z_ARRVAL_P(target));
  for (zend_long i = 0; i < target_size; i++) {
    Bucket *carry = Z_ARRVAL_P(target)->arData + i;
    ZVAL_COPY(&value, &carry->val);
    zend_hash_next_index_insert(Z_ARRVAL(array), &value);
  }
  Z_ARRVAL(array)->nNumUsed = current_size + target_size;
  Z_ARRVAL(array)->nNextFreeElement = current_size + target_size;
  zend_hash_internal_pointer_reset(Z_ARRVAL(array));
  RETURN_ARR(Z_ARRVAL(array));
}

PHP_METHOD(_array, indexOf)
{
  zval *_value;
  ZEND_PARSE_PARAMETERS_START(1, 1)
    Z_PARAM_ZVAL(_value)
  ZEND_PARSE_PARAMETERS_END();
  zval *_array = vary_array_getValue(getThis());
  zend_long result = vary_array_indexOf(_array, _value, 0);
  RETURN_LONG(result);
}

PHP_METHOD(_array, lastIndexOf)
{
  zval *_value;
  ZEND_PARSE_PARAMETERS_START(1, 1)
    Z_PARAM_ZVAL(_value)
  ZEND_PARSE_PARAMETERS_END();
  zval *_array = vary_array_getValue(getThis());
  zend_long result = vary_array_indexOf(_array, _value, 1);
  RETURN_LONG(result);
}

PHP_METHOD(_array, includes)
{
  zval *_value;
  ZEND_PARSE_PARAMETERS_START(1, 1)
    Z_PARAM_ZVAL(_value)
  ZEND_PARSE_PARAMETERS_END();
  zval *_array = vary_array_getValue(getThis());
  zend_long result = vary_array_indexOf(_array, _value, 1);
  if (result == -1) {
    RETURN_FALSE;
  }
  RETURN_TRUE;
}

PHP_METHOD(_array, every)
{
  zend_fcall_info user_func = empty_fcall_info;
  zend_fcall_info_cache user_func_cache = empty_fcall_info_cache;
  ZEND_PARSE_PARAMETERS_START(1, 1)
    Z_PARAM_FUNC(user_func, user_func_cache)
  ZEND_PARSE_PARAMETERS_END();
  zval *_array = vary_array_getValue(getThis());
  zend_long array_size = zend_hash_num_elements(Z_ARRVAL_P(_array));
  zend_long result = 1;
  for (zend_long i = 0; i < array_size; ++i) {
    Bucket *current_item = Z_ARRVAL_P(_array)->arData + i;
    zval retval;
    zval args[1];
    ZVAL_COPY(&args[0], &current_item->val);
    user_func.retval = &retval;
    user_func.param_count = 1;
    user_func.no_separation = 0;
    user_func.params = args;
    zend_call_function(&user_func, &user_func_cache);
    zval_ptr_dtor(&args[0]);
    if (Z_TYPE(retval) == IS_FALSE) {
      zval_ptr_dtor(&retval);
      result = 0;
      break;
    }
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
  zend_fcall_info user_func = empty_fcall_info;
  zend_fcall_info_cache user_func_cache = empty_fcall_info_cache;
  ZEND_PARSE_PARAMETERS_START(1, 1)
    Z_PARAM_FUNC(user_func, user_func_cache)
  ZEND_PARSE_PARAMETERS_END();
  zval *_array = vary_array_getValue(getThis());
  zend_long array_size = zend_hash_num_elements(Z_ARRVAL_P(_array));
  zend_long result = 0;
  for (zend_long i = 0; i < array_size; ++i) {
    Bucket *current_item = Z_ARRVAL_P(_array)->arData + i;
    zval retval;
    zval args[1];
    ZVAL_COPY(&args[0], &current_item->val);
    user_func.retval = &retval;
    user_func.param_count = 1;
    user_func.no_separation = 0;
    user_func.params = args;
    zend_call_function(&user_func, &user_func_cache);
    zval_ptr_dtor(&args[0]);
    if (Z_TYPE(retval) == IS_TRUE) {
      zval_ptr_dtor(&retval);
      result = 1;
      break;
    }
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
  zend_fcall_info user_func = empty_fcall_info;
  zend_fcall_info_cache user_func_cache = empty_fcall_info_cache;
  ZEND_PARSE_PARAMETERS_START(1, 1)
    Z_PARAM_FUNC(user_func, user_func_cache)
  ZEND_PARSE_PARAMETERS_END();
  zval *_array = vary_array_getValue(getThis());
  zend_long array_size = zend_hash_num_elements(Z_ARRVAL_P(_array));
  zval result;
  array_init(&result);
  for (zend_long i = 0; i < array_size; ++i) {
    Bucket *carry = Z_ARRVAL_P(_array)->arData + i;
    zval args[1], retval;
    ZVAL_COPY(&args[0], &carry->val);
    user_func.retval = &retval;
    user_func.param_count = 1;
    user_func.no_separation = 0;
    user_func.params = args;
    zend_call_function(&user_func, &user_func_cache);
    zend_hash_next_index_insert(Z_ARRVAL(result), &retval);
    zval_ptr_dtor(&args[0]);
  }
  Z_ARRVAL(result)->nNumUsed = array_size;
  Z_ARRVAL(result)->nNextFreeElement = array_size;
  zend_hash_internal_pointer_reset(Z_ARRVAL(result));
  RETURN_ARR(Z_ARRVAL(result));
}

PHP_METHOD(_array, fill)
{
  zval *_value, value;
  zend_long start = 0, end;
  ZEND_PARSE_PARAMETERS_START(1, 3)
    Z_PARAM_ZVAL(_value)
    Z_PARAM_OPTIONAL
    Z_PARAM_LONG(start)
    Z_PARAM_LONG(end)
  ZEND_PARSE_PARAMETERS_END();
  zval *_array = vary_array_getValue(getThis());
  zend_long array_size = zend_hash_num_elements(Z_ARRVAL_P(_array));
  if (ZEND_NUM_ARGS() != 3 || end > array_size) {
    end = array_size;
  }
  zval result;
  array_init(&result);
  ZVAL_COPY(&value, _value);
  for (zend_long i = 0; i < array_size; ++i) {
    Bucket *current = Z_ARRVAL_P(_array)->arData + i;
    if (i >= start && i < end) {
      zend_hash_next_index_insert(Z_ARRVAL(result), &value);
    } else {
      zend_hash_next_index_insert(Z_ARRVAL(result), &current->val);
      ZVAL_UNDEF(&current->val);
    }
  }
  Z_ARRVAL(result)->nNumUsed = array_size;
  Z_ARRVAL(result)->nNextFreeElement = array_size;
  zend_hash_internal_pointer_reset(Z_ARRVAL(result));
  vary_array_setValue(getThis(), result);
  zval_ptr_dtor(&result);
  RETURN_TRUE;
}

PHP_METHOD(_array, forEach)
{
  zend_fcall_info user_func = empty_fcall_info;
  zend_fcall_info_cache user_func_cache = empty_fcall_info_cache;
  ZEND_PARSE_PARAMETERS_START(1, 1)
    Z_PARAM_FUNC(user_func, user_func_cache)
  ZEND_PARSE_PARAMETERS_END();
  zval *_array = vary_array_getValue(getThis());
  zend_long array_size = zend_hash_num_elements(Z_ARRVAL_P(_array));
  for (zend_long i = 0; i < array_size; ++i) {
    Bucket *carry = Z_ARRVAL_P(_array)->arData + i;
    zval args[1], retval;
    ZVAL_COPY(&args[0], &carry->val);
    user_func.retval = &retval;
    user_func.param_count = 1;
    user_func.no_separation = 0;
    user_func.params = args;
    zend_call_function(&user_func, &user_func_cache);
    zval_ptr_dtor(&args[0]);
    zval_ptr_dtor(&retval);
  }
  RETURN_NULL();
}

PHP_METHOD(_array, reduce)
{
  zend_fcall_info user_func = empty_fcall_info;
  zend_fcall_info_cache user_func_cache = empty_fcall_info_cache;
  zval *start_value;
  ZEND_PARSE_PARAMETERS_START(1, 2)
    Z_PARAM_FUNC(user_func, user_func_cache)
    Z_PARAM_OPTIONAL
    Z_PARAM_ZVAL(start_value);
  ZEND_PARSE_PARAMETERS_END();
  zval sum_value = vary_array_reduce(
    vary_array_getValue(getThis()),
    user_func,
    user_func_cache,
    ZEND_NUM_ARGS(),
    start_value,
    0
  );
  RETURN_ZVAL(&sum_value, 0, 1);
}

PHP_METHOD(_array, reduceRight)
{
  zend_fcall_info user_func = empty_fcall_info;
  zend_fcall_info_cache user_func_cache = empty_fcall_info_cache;
  zval *start_value;
  ZEND_PARSE_PARAMETERS_START(1, 2)
    Z_PARAM_FUNC(user_func, user_func_cache)
    Z_PARAM_OPTIONAL
    Z_PARAM_ZVAL(start_value);
  ZEND_PARSE_PARAMETERS_END();
  zval sum_value = vary_array_reduce(
    vary_array_getValue(getThis()),
    user_func,
    user_func_cache,
    ZEND_NUM_ARGS(),
    start_value,
    1
  );
  RETURN_ZVAL(&sum_value, 0, 1);
}

PHP_METHOD(_array, findIndex)
{
  zend_fcall_info user_func = empty_fcall_info;
  zend_fcall_info_cache user_func_cache = empty_fcall_info_cache;
  ZEND_PARSE_PARAMETERS_START(1, 1)
    Z_PARAM_FUNC(user_func, user_func_cache)
  ZEND_PARSE_PARAMETERS_END();
  zval *_array = vary_array_getValue(getThis());
  zend_long index = vary_array_findIndex(_array, user_func, user_func_cache);
  RETURN_LONG(index);
}

PHP_METHOD(_array, find)
{
  zend_fcall_info user_func = empty_fcall_info;
  zend_fcall_info_cache user_func_cache = empty_fcall_info_cache;
  ZEND_PARSE_PARAMETERS_START(1, 1)
    Z_PARAM_FUNC(user_func, user_func_cache)
  ZEND_PARSE_PARAMETERS_END();
  zval *_array = vary_array_getValue(getThis());
  zend_long index = vary_array_findIndex(_array, user_func, user_func_cache);
  if (index == -1) {
    RETURN_NULL();
  }
  Bucket *target = Z_ARRVAL_P(_array)->arData + index;
  RETURN_ZVAL(&target->val, 1, 0);
}

PHP_METHOD(_array, filter)
{
  zend_fcall_info user_func = empty_fcall_info;
  zend_fcall_info_cache user_func_cache = empty_fcall_info_cache;
  ZEND_PARSE_PARAMETERS_START(1, 1)
    Z_PARAM_FUNC(user_func, user_func_cache)
  ZEND_PARSE_PARAMETERS_END();
  zval *_array = vary_array_getValue(getThis());
  zend_long array_size = zend_hash_num_elements(Z_ARRVAL_P(_array));
  zend_long filtered_size = 0;
  zval result;
  array_init(&result);
  for (zend_long i = 0; i < array_size; ++i) {
    Bucket *carry = Z_ARRVAL_P(_array)->arData + i;
    zval args[1], retval;
    ZVAL_COPY(&args[0], &carry->val);
    user_func.retval = &retval;
    user_func.param_count = 1;
    user_func.no_separation = 0;
    user_func.params = args;
    zend_call_function(&user_func, &user_func_cache);
    if (Z_TYPE(retval) == IS_TRUE) {
      zend_hash_next_index_insert(Z_ARRVAL(result), &args[0]);
      ZVAL_UNDEF(&args[0]);
      ++filtered_size;
    }
    zval_ptr_dtor(&args[0]);
    zval_ptr_dtor(&retval);
  }
  Z_ARRVAL(result)->nNumUsed = filtered_size;
  Z_ARRVAL(result)->nNextFreeElement = filtered_size;
  zend_hash_internal_pointer_reset(Z_ARRVAL(result));
  RETURN_ARR(Z_ARRVAL(result));
}

PHP_METHOD(_array, slice)
{
  zend_long start_index, end_index;
  ZEND_PARSE_PARAMETERS_START(1, 2)
    Z_PARAM_LONG(start_index)
    Z_PARAM_OPTIONAL
    Z_PARAM_LONG(end_index)
  ZEND_PARSE_PARAMETERS_END();
  zval *array = vary_array_getValue(getThis());
  zend_long array_size = zend_hash_num_elements(Z_ARRVAL_P(array));
  zval result;
  array_init(&result);
  if (ZEND_NUM_ARGS() == 1 || array_size < end_index) {
    end_index = array_size;
  }
  zval value;
  for (zend_long i = start_index; i < end_index; ++i) {
    Bucket *current = Z_ARRVAL_P(array)->arData + i;
    ZVAL_COPY(&value, &current->val);
    zend_hash_next_index_insert(Z_ARRVAL(result), &value);
  }
  RETURN_ARR(Z_ARRVAL(result));
}

PHP_METHOD(_array, splice)
{
  zend_long start_index, remove_size;
  zval *_value;
  ZEND_PARSE_PARAMETERS_START(2, 3)
    Z_PARAM_LONG(start_index)
    Z_PARAM_LONG(remove_size)
    Z_PARAM_OPTIONAL
    Z_PARAM_ZVAL(_value)
  ZEND_PARSE_PARAMETERS_END();
  zval *array = vary_array_getValue(getThis());
  zval result;
  array_init(&result);
  zend_long i;
  zend_long array_size = zend_hash_num_elements(Z_ARRVAL_P(array));
  for (i = 0; i < start_index && i < array_size; ++i) {
    Bucket *current = Z_ARRVAL_P(array)->arData + i;
    zend_hash_next_index_insert(Z_ARRVAL(result), &current->val);
    ZVAL_UNDEF(&current->val);
  }
  if (ZEND_NUM_ARGS() == 3) {
    zval value;
    ZVAL_COPY(&value, _value);
    zend_hash_next_index_insert(Z_ARRVAL(result), &value);
    ZVAL_UNDEF(&value);
    zval_ptr_dtor(&value);
  }
  for (i = start_index + remove_size; i < array_size; ++i) {
    Bucket *current = Z_ARRVAL_P(array)->arData + i;
    zend_hash_next_index_insert(Z_ARRVAL(result), &current->val);
    ZVAL_UNDEF(&current->val);
  }
  vary_array_setValue(getThis(), result);
  zval_ptr_dtor(&result);
  RETURN_TRUE;
}

PHP_METHOD(OrderedList, __construct)
{
  ZEND_PARSE_PARAMETERS_START(0, 0)
  ZEND_PARSE_PARAMETERS_END();
  zval array;
  array_init(&array);
  vary_array_setValue(getThis(), array);
  zval_ptr_dtor(&array);
  RETURN_TRUE;
}

PHP_METHOD(OrderedList, add)
{
  zend_long new_value;
  ZEND_PARSE_PARAMETERS_START(1, 1)
    Z_PARAM_LONG(new_value)
  ZEND_PARSE_PARAMETERS_END();
  zval *_array = vary_array_getValue(getThis());
  zval _binarySearch_retval = vary_algorithm_binarySearch(_array, new_value);
  Bucket *carry = Z_ARRVAL_P(&_binarySearch_retval)->arData + 1;
  if (zval_get_long(&carry->val) == 1) {
    zval_ptr_dtor(&_binarySearch_retval);
    RETURN_FALSE;
  }
  carry = Z_ARRVAL_P(&_binarySearch_retval)->arData;
  zend_long target_index = zval_get_long(&carry->val);
  zval_ptr_dtor(&_binarySearch_retval);
  zend_long array_size = zend_hash_num_elements(Z_ARRVAL_P(_array));
  zval value;
  if (target_index == array_size) {
    ZVAL_LONG(&value, new_value);
    zend_hash_next_index_insert(Z_ARRVAL_P(_array), &value);
  } else {
    carry = Z_ARRVAL_P(_array)->arData + array_size - 1;
    ZVAL_COPY(&value, &carry->val);
    zend_hash_next_index_insert(Z_ARRVAL_P(_array), &value);
    if (array_size >= 2) {
      for (zend_long i = array_size - 2; i >= target_index; i--) {
        carry = Z_ARRVAL_P(_array)->arData + i;
        Bucket *holder = Z_ARRVAL_P(_array)->arData + i + 1;
        ZVAL_COPY_VALUE(&holder->val, &carry->val);
      }
    }
    carry = Z_ARRVAL_P(_array)->arData + target_index;
    ZVAL_LONG(&value, new_value);
    ZVAL_COPY_VALUE(&carry->val, &value);
  }
  Z_ARRVAL_P(_array)->nNumUsed = array_size + 1;
  Z_ARRVAL_P(_array)->nNextFreeElement = array_size + 1;
  zend_hash_internal_pointer_reset(Z_ARRVAL_P(_array));
  zval_ptr_dtor(&value);
  RETURN_LONG(target_index);
}

PHP_METHOD(OrderedList, remove)
{
  zend_long target_value;
  ZEND_PARSE_PARAMETERS_START(1, 1)
    Z_PARAM_LONG(target_value)
  ZEND_PARSE_PARAMETERS_END();
  zval *_array = vary_array_getValue(getThis());
  zval _binarySearch_retval = vary_algorithm_binarySearch(_array, target_value);
  Bucket *carry = Z_ARRVAL_P(&_binarySearch_retval)->arData + 1;
  if (zval_get_long(&carry->val) == 0) {
    zval_ptr_dtor(&_binarySearch_retval);
    RETURN_FALSE;
  }
  carry = Z_ARRVAL_P(&_binarySearch_retval)->arData;
  zend_long target_index = zval_get_long(&carry->val);
  zval_ptr_dtor(&_binarySearch_retval);
  zval return_item = vary_array_removeIndex(_array, target_index);
  zval_ptr_dtor(&return_item);
  RETURN_LONG(target_index);
}

PHP_METHOD(OrderedList, indexOf)
{
  zend_long target_value;
  ZEND_PARSE_PARAMETERS_START(1, 1)
    Z_PARAM_LONG(target_value)
  ZEND_PARSE_PARAMETERS_END();
  zval *_array = vary_array_getValue(getThis());
  zval _binarySearch_retval = vary_algorithm_binarySearch(_array, target_value);
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


PHP_METHOD(SetList, __construct)
{
  ZEND_PARSE_PARAMETERS_START(0, 0)
  ZEND_PARSE_PARAMETERS_END();
  zval array;
  array_init(&array);
  vary_array_setValue(getThis(), array);
  zval_ptr_dtor(&array);
  RETURN_TRUE;
}

PHP_METHOD(SetList, size)
{
  ZEND_PARSE_PARAMETERS_START(0, 0)
  ZEND_PARSE_PARAMETERS_END();
  zval *_array = vary_array_getValue(getThis());
  RETURN_LONG(zend_hash_num_elements(Z_ARRVAL_P(_array)));
}
