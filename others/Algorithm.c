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