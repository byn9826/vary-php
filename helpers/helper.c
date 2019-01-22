#include "php.h"

zval vary_binarySearch(zval *array, zend_long value) {
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

zval *vary_getItems(zval *this) {
  zval *rv;
  return zend_read_property(
    _array_handle,
    this,
    "_items",
    sizeof("_items") - 1,
    0,
    rv TSRMLS_CC
  );
}