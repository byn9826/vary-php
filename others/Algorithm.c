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
  ZEND_PARSE_PARAMETERS_START(1, 1)
    Z_PARAM_ARRAY(_array)
  ZEND_PARSE_PARAMETERS_END();
  uint32_t full_size = zend_hash_num_elements(Z_ARRVAL_P(_array));
  uint32_t sub_size = full_size / 2;
  zval carry;
  zend_long current_value;
  while (sub_size > 0) {
    for (uint32_t start = 0; start < sub_size; start++) {
      for (uint32_t i = start + sub_size; i < full_size; i += sub_size) {
        uint32_t position = i;
        Bucket *current_item = Z_ARRVAL_P(_array)->arData + position;
        Bucket *target_item = Z_ARRVAL_P(_array)->arData + position - sub_size;
        current_value = Z_LVAL_P(&current_item->val);
        while (position >= sub_size && Z_LVAL_P(&target_item->val) > current_value) {
          ZVAL_COPY_VALUE(&carry, &target_item->val);
          ZVAL_COPY_VALUE(&target_item->val, &current_item->val);
          ZVAL_COPY_VALUE(&current_item->val, &carry);
          position -= sub_size;
          current_item = Z_ARRVAL_P(_array)->arData + position;
          target_item = Z_ARRVAL_P(_array)->arData + position - sub_size;
        }
      }
    }
    sub_size /= 2;
  }
  zend_hash_internal_pointer_reset(Z_ARRVAL_P(_array));
  if (full_size != 0) {
    zval_ptr_dtor(&carry);
  }
  RETURN_TRUE;
}