#include "php.h"

ZEND_BEGIN_ARG_INFO(arginfo_void, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_any, 0)
  ZEND_ARG_INFO(0, _value)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_integer, 0)
  ZEND_ARG_INFO(0, _value)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_array, 0)
  ZEND_ARG_INFO(0, _value)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_array_integer, 0)
  ZEND_ARG_INFO(0, _array)
  ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_function, 0)
  ZEND_ARG_INFO(0, func)
ZEND_END_ARG_INFO()


ZEND_BEGIN_ARG_INFO(arginfo_function_any, 0)
  ZEND_ARG_INFO(0, func)
  ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_array_function, 0)
  ZEND_ARG_INFO(0, _array)
  ZEND_ARG_INFO(0, func)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_any_integer_integer, 0)
  ZEND_ARG_INFO(0, value)
  ZEND_ARG_INFO(0, start)
  ZEND_ARG_INFO(0, end)
ZEND_END_ARG_INFO()
