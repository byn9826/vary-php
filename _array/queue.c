#include "php.h"

zend_class_entry *queue_handle;

PHP_METHOD(Queue, enqueue)
{
  int num_args = ZEND_CALL_NUM_ARGS(execute_data);
  if (num_args != 1) {
    warning_params_exact_one("\\Vary\\Queue enqueue() method");
  }
  zval *_value;
  ZEND_PARSE_PARAMETERS_START(1, 1)
    Z_PARAM_ZVAL(_value)
  ZEND_PARSE_PARAMETERS_END();
  zval *this = getThis();
  zval retval = _array_addToBack(this, _value);
  RETURN_BOOL(&retval);
}

PHP_METHOD(Queue, dequeue)
{
  int num_args = ZEND_CALL_NUM_ARGS(execute_data);
  if (num_args != 0) {
    warning_params_exact_zero("\\Vary\\Queue dequeue() method");
  }
  zval *this = getThis();
  zval retval = _array__deleteFromFront(this);
  RETURN_ZVAL(&retval, 1, 1);
}