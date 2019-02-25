#include "php.h"

zend_class_entry *algorithm_handle;

zval algorithm_binarySearch(zval *array, zend_long value) {
  zval _binarySearch_name, _binarySearch_retval;
  ZVAL_STRING(&_binarySearch_name, "\\Vary\\Algorithm::binarySearch");
  zval params[2];
  ZVAL_COPY(&params[0], array);
  ZVAL_LONG(&params[1], value);
  call_user_function(
    EG(function_table),
    NULL,
    &_binarySearch_name,
    &_binarySearch_retval,
    2,
    params TSRMLS_CC
  );
  zval_ptr_dtor(&_binarySearch_name);
  zval_ptr_dtor(params);
  return _binarySearch_retval;
}

PHP_METHOD(Algorithm, binarySearch)
{
  zval *_array;
  zend_long value;
  ZEND_PARSE_PARAMETERS_START(2, 2)
    Z_PARAM_ARRAY(_array)
    Z_PARAM_LONG(value)
  ZEND_PARSE_PARAMETERS_END();
  uint32_t items_size = zend_hash_num_elements(Z_ARRVAL_P(_array));
  int first = 0;
  int last = items_size - 1;
  int found = 0;
  int middle;
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
  RETURN_ZVAL(&rtval, 0, 1);
}

void _insertion_sort(zval *array, int start, int gap, int size)
{
  
}

PHP_METHOD(Algorithm, shellSort)
{
  zval *_array;
  zend_fcall_info user_compare_func;
	zend_fcall_info_cache user_compare_func_cache = empty_fcall_info_cache;
  ZEND_PARSE_PARAMETERS_START(1, 2)
    Z_PARAM_ARRAY(_array)
    Z_PARAM_OPTIONAL
    Z_PARAM_FUNC(user_compare_func, user_compare_func_cache)
  ZEND_PARSE_PARAMETERS_END();
  uint32_t full_size = zend_hash_num_elements(Z_ARRVAL_P(_array));
  uint32_t sub_size = full_size / 2;
  while (sub_size > 0) {
    for (uint32_t start = 0; start < sub_size; start++) {
      for (uint32_t i = start + sub_size; i < full_size; i += sub_size) {
        uint32_t position = i;
        Bucket *current_item = Z_ARRVAL_P(_array)->arData + position;
        Bucket *target_item = Z_ARRVAL_P(_array)->arData + position - sub_size;
        zval current_value;
        ZVAL_COPY_UNREF(&current_value, &current_item->val);
        while (position >= sub_size) {
          zval target_value, compare_result;
          ZVAL_COPY_UNREF(&target_value, &target_item->val);
          if (ZEND_NUM_ARGS() == 2) {
            zval retval;
            zval args[2];
            ZVAL_COPY(&args[0], &target_value);
            ZVAL_COPY(&args[1], &current_value);
            user_compare_func.retval = &retval;
            user_compare_func.param_count = 2;
            user_compare_func.no_separation = 0;
		        user_compare_func.params = args;
            if (
              zend_call_function(&user_compare_func, &user_compare_func_cache) == SUCCESS
              && Z_TYPE(retval) == IS_TRUE
            ) {
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
  RETURN_TRUE;
}