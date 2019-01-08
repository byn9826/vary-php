#include "php.h"

#define warning_params_max_one(msg) php_error_docref0(NULL, E_WARNING, "%s requires at most one parameter", msg)
#define warning_params_exact_zero(msg) php_error_docref0(NULL, E_WARNING, "%s requires no parameter", msg)
#define warning_params_exact_one(msg) php_error_docref0(NULL, E_WARNING, "%s requires one parameter", msg)

ZEND_BEGIN_ARG_INFO(arginfo_void, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_single, 0)
  ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_array, 0)
  ZEND_ARG_INFO(0, array)
ZEND_END_ARG_INFO()