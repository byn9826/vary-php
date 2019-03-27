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

PHP_METHOD(_map, value)
{
  ZEND_PARSE_PARAMETERS_START(0, 0)
  ZEND_PARSE_PARAMETERS_END();
  zval *_map = vary_map_getValue(getThis());
  zval map;
  ZVAL_COPY(&map, _map);
  RETURN_ARR(Z_ARRVAL(map));
}

PHP_METHOD(_map, size)
{
  ZEND_PARSE_PARAMETERS_START(0, 0)
  ZEND_PARSE_PARAMETERS_END();
  zval *_map = vary_map_getValue(getThis());
  RETURN_LONG(zend_hash_num_elements(Z_ARRVAL_P(_map)));
}

PHP_METHOD(_map, setValue)
{
  zval map, *_map;
  ZEND_PARSE_PARAMETERS_START(1, 1)
    Z_PARAM_ARRAY(_map)
  ZEND_PARSE_PARAMETERS_END();
  ZVAL_COPY(&map, _map);
  vary_array_setValue(getThis(), map);
  zval_ptr_dtor(&map);
  RETURN_TRUE;
}