#include "php.h"

zend_class_entry *_map_handle;
zend_class_entry *mapList_handle;

zval *vary_map_getValue(zval *this)
{
  return vary_lists_getValue(_map_handle, this);
}

void vary_map_setValue(zval *this, zval value)
{
  vary_lists_setValue(_map_handle, this, value);
}

PHP_METHOD(_map, __construct)
{
  zval map, *_map;
  ZEND_PARSE_PARAMETERS_START(0, 1)
    Z_PARAM_OPTIONAL
    Z_PARAM_ARRAY(_map)
  ZEND_PARSE_PARAMETERS_END();
  if (ZEND_NUM_ARGS() == 0) {
    array_init(&map);
  } else {
    ZVAL_COPY(&map, _map);
  }
  vary_map_setValue(getThis(), map);
  zval_ptr_dtor(&map);
  RETURN_TRUE;
}