#include "php.h"

ZEND_BEGIN_ARG_INFO(arginfo_void, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_any, 0)
  ZEND_ARG_INFO(0, _value)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_array, 0)
  ZEND_ARG_INFO(0, _value)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_array_integer, 0)
  ZEND_ARG_INFO(0, _array)
  ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()