#include "php.h"

/*
 * Common methods for lists classes
 */
zval *vary_lists_getValue(zend_class_entry *handle, zval *this)
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

void vary_lists_setValue(zend_class_entry *handle, zval *this, zval value)
{
  zend_update_property(
    handle,
    this,
    "__value__",
    sizeof("__value__") - 1,
    &value TSRMLS_CC
  );
}