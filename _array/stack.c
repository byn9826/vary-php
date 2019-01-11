#include "php.h"

zend_class_entry *stack_handle;

PHP_METHOD(Stack, push)
{
  int num_args = ZEND_CALL_NUM_ARGS(execute_data);
  if (num_args != 1) {
    warning_params_exact_one("\\Vary\\Stack push() method");
  }
  zval *_value;
  ZEND_PARSE_PARAMETERS_START(1, 1)
    Z_PARAM_ZVAL(_value)
  ZEND_PARSE_PARAMETERS_END();
  zval *this = getThis();
  zval retval = _array_addToBack(this, _value);
  RETURN_BOOL(&retval);
}

PHP_METHOD(Stack, pop)
{
  int num_args = ZEND_CALL_NUM_ARGS(execute_data);
  if (num_args != 0) {
    warning_params_exact_zero("\\Vary\\Stack pop() method");
  }
  zval *this = getThis();
  zval retval = _array_deleteFromBack(this);
  RETURN_ZVAL(&retval, 1, 1);
}