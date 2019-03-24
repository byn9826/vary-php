#include "php.h"

ZEND_BEGIN_ARG_INFO(arginfo_void, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_any, 0)
  ZEND_ARG_INFO(0, any)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_integer, 0)
  ZEND_ARG_INFO(0, integer)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_array, 0)
  ZEND_ARG_INFO(0, array)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_array_integer, 0)
  ZEND_ARG_INFO(0, array)
  ZEND_ARG_INFO(0, integer)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_function, 0)
  ZEND_ARG_INFO(0, function)
ZEND_END_ARG_INFO()


ZEND_BEGIN_ARG_INFO(arginfo_function_any, 0)
  ZEND_ARG_INFO(0, function)
  ZEND_ARG_INFO(0, any)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_array_function, 0)
  ZEND_ARG_INFO(0, array)
  ZEND_ARG_INFO(0, function)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_any_integer_integer, 0)
  ZEND_ARG_INFO(0, any)
  ZEND_ARG_INFO(0, integer)
  ZEND_ARG_INFO(0, integer)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_integer_integer, 0)
  ZEND_ARG_INFO(0, integer)
  ZEND_ARG_INFO(0, integer)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_integer_integer_any, 0)
  ZEND_ARG_INFO(0, integer)
  ZEND_ARG_INFO(0, integer)
  ZEND_ARG_INFO(0, any)
ZEND_END_ARG_INFO()
