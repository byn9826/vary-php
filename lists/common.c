#include "php.h"

/*
 * Common methods for list classes
 */
zval *vary_list_getValue(zend_class_entry *handle, zval *this)
{
  zval *rv;
  return zend_read_property(
    handle,
    this,
    "__value__",
    sizeof("__value__") - 1,
    0,
    rv TSRMLS_CC
  );
}

void vary_list_setValue(zend_class_entry *handle, zval *this, zval value)
{
  zend_update_property(
    handle,
    this,
    "__value__",
    sizeof("__value__") - 1,
    &value TSRMLS_CC
  );
}

void vary_list_init_normal(INTERNAL_FUNCTION_PARAMETERS, zend_long type)
{
  zval list, *_list;
  ZEND_PARSE_PARAMETERS_START(0, 1)
    Z_PARAM_OPTIONAL
    Z_PARAM_ARRAY(_list)
  ZEND_PARSE_PARAMETERS_END();
  if (ZEND_NUM_ARGS() == 0) {
    array_init(&list);
  } else {
    ZVAL_COPY(&list, _list);
  }
  if (type == 0) {
    vary_list_setValue(_array_handle, getThis(), list);
  } else {
    vary_list_setValue(_dict_handle, getThis(), list);
  }
  zval_ptr_dtor(&list);
  RETURN_TRUE;
}

void vary_list_init_empty(INTERNAL_FUNCTION_PARAMETERS, zend_long type)
{
  ZEND_PARSE_PARAMETERS_START(0, 0)
  ZEND_PARSE_PARAMETERS_END();
  zval list;
  array_init(&list);
  if (type == 0) {
    vary_list_setValue(_array_handle, getThis(), list);
  } else {
    vary_list_setValue(_dict_handle, getThis(), list);
  }
  zval_ptr_dtor(&list);
  RETURN_TRUE;
}

void vary_list_length(INTERNAL_FUNCTION_PARAMETERS, zend_long type)
{
  ZEND_PARSE_PARAMETERS_START(0, 0)
  ZEND_PARSE_PARAMETERS_END();
  zval *_list;
  if (type == 0) {
    _list = vary_list_getValue(_array_handle, getThis());
  } else {
    _list = vary_list_getValue(_dict_handle, getThis());
  }
  RETURN_LONG(zend_hash_num_elements(Z_ARRVAL_P(_list)));
}