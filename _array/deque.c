#include "php.h"

zend_class_entry *deque_handle;

PHP_METHOD(Deque, addFront)
{
  int num_args = ZEND_CALL_NUM_ARGS(execute_data);
  if (num_args != 1) {
    warning_params_exact_one("\\Vary\\Deque addFront() method");
  }
  zval *_value;
  ZEND_PARSE_PARAMETERS_START(1, 1)
    Z_PARAM_ZVAL(_value)
  ZEND_PARSE_PARAMETERS_END();
  zval *this = getThis();
  zval retval = _array_addToFront(this, _value);
  RETURN_BOOL(&retval);
}

PHP_METHOD(Deque, addRear)
{
  int num_args = ZEND_CALL_NUM_ARGS(execute_data);
  if (num_args != 1) {
    warning_params_exact_one("\\Vary\\Deque addRear() method");
  }
  zval *_value;
  ZEND_PARSE_PARAMETERS_START(1, 1)
    Z_PARAM_ZVAL(_value)
  ZEND_PARSE_PARAMETERS_END();
  zval *this = getThis();
  zval retval = _array_addToBack(this, _value);
  RETURN_BOOL(&retval);
}

PHP_METHOD(Deque, removeRear)
{
  int num_args = ZEND_CALL_NUM_ARGS(execute_data);
  if (num_args != 0) {
    warning_params_exact_zero("\\Vary\\Deque removeRear() method");
  }
  zval *this = getThis();
  zval retval = _array_deleteFromBack(this);
  RETURN_ZVAL(&retval, 1, 1);
}

PHP_METHOD(Deque, removeFront)
{
  int num_args = ZEND_CALL_NUM_ARGS(execute_data);
  if (num_args != 0) {
    warning_params_exact_zero("\\Vary\\Deque removeFront() method");
  }
  zval *this = getThis();
  zval retval = _array__deleteFromFront(this);
  RETURN_ZVAL(&retval, 1, 1);
}