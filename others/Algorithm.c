#include "php.h"

zend_class_entry *algorithm_handle;

void vary_algorithm_shellSort(
  zval *_array,
  zend_fcall_info user_func,
  zend_fcall_info_cache user_func_cache,
  zend_long params_num
)
{
  zend_long full_size = zend_hash_num_elements(Z_ARRVAL_P(_array));
  zend_long sub_size = full_size / 2;
  while (sub_size > 0) {
    for (zend_long start = 0; start < sub_size; start++) {
      for (zend_long i = start + sub_size; i < full_size; i += sub_size) {
        zend_long position = i;
        Bucket *current_item = Z_ARRVAL_P(_array)->arData + position;
        Bucket *target_item = Z_ARRVAL_P(_array)->arData + position - sub_size;
        zval current_value;
        ZVAL_COPY(&current_value, &current_item->val);
        while (position >= sub_size) {
          zval target_value, compare_result;
          ZVAL_COPY(&target_value, &target_item->val);
          if (params_num == 2) {
            zval args[2], retval;
            ZVAL_COPY(&args[0], &target_value);
            ZVAL_COPY(&args[1], &current_value);
            user_func.retval = &retval;
            user_func.param_count = 2;
            user_func.no_separation = 0;
            user_func.params = args;
            zend_call_function(&user_func, &user_func_cache);
            if (Z_TYPE(retval) == IS_TRUE) {
              zval_ptr_dtor(&args[0]);
              zval_ptr_dtor(&args[1]);
              zval_ptr_dtor(&target_value);
              zval_ptr_dtor(&retval);
              break;
            } else {
              zval_ptr_dtor(&args[0]);
              zval_ptr_dtor(&args[1]);
              zval_ptr_dtor(&retval);
            }
          } else {
            compare_function(&compare_result, &target_value, &current_value);
            if (Z_LVAL_P(&compare_result) < 0) {
              zval_ptr_dtor(&compare_result);
              zval_ptr_dtor(&target_value);
              break;
            }
          }
          ZVAL_COPY_VALUE(&target_item->val, &current_item->val);
          ZVAL_COPY_VALUE(&current_item->val, &target_value);
          position -= sub_size;
          current_item = Z_ARRVAL_P(_array)->arData + position;
          target_item = Z_ARRVAL_P(_array)->arData + position - sub_size;
          zval_ptr_dtor(&target_value);
          zval_ptr_dtor(&compare_result);
        }
        zval_ptr_dtor(&current_value);
      }
    }
    sub_size /= 2;
  }
  zend_hash_internal_pointer_reset(Z_ARRVAL_P(_array));
}

zval vary_algorithm_binarySearch(zval *_array, zend_long value)
{
  zend_long array_size = zend_hash_num_elements(Z_ARRVAL_P(_array));
  zend_long first = 0;
  zend_long last = array_size - 1;
  zend_long found = 0;
  zend_long middle;
  Bucket *item;
  while (first <= last && found == 0) {
    middle = (first + last) / 2;
    item = Z_ARRVAL_P(_array)->arData + middle;
    if (value == Z_LVAL(item->val)) {
      found = 1;
    } else {
      if (value < Z_LVAL(item->val)) {
        last = middle - 1;
      } else {
        first = middle + 1;
      }
    }
  }
  zval rtval;
  array_init(&rtval);
  add_assoc_long(&rtval, "index", found == 1 ? middle : first);
  add_assoc_bool(&rtval, "exist", found);
  return rtval;
}

PHP_METHOD(Algorithm, binarySearch)
{
  zval *_array;
  zend_long value;
  ZEND_PARSE_PARAMETERS_START(2, 2)
    Z_PARAM_ARRAY(_array)
    Z_PARAM_LONG(value)
  ZEND_PARSE_PARAMETERS_END();
  zval rtval = vary_algorithm_binarySearch(_array, value);
  RETURN_ZVAL(&rtval, 0, 1);
}

PHP_METHOD(Algorithm, shellSort)
{
  zval *_array;
  zend_fcall_info user_func = empty_fcall_info;
  zend_fcall_info_cache user_func_cache = empty_fcall_info_cache;
  ZEND_PARSE_PARAMETERS_START(1, 2)
    Z_PARAM_ARRAY(_array)
    Z_PARAM_OPTIONAL
    Z_PARAM_FUNC(user_func, user_func_cache)
  ZEND_PARSE_PARAMETERS_END();
  vary_algorithm_shellSort(_array, user_func, user_func_cache, ZEND_NUM_ARGS());
}