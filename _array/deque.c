#include "php.h"

zend_class_entry *deque_handle;

PHP_METHOD(Deque, addFront)
{
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
  ZEND_PARSE_PARAMETERS_START(0, 0)
  ZEND_PARSE_PARAMETERS_END();
  zval *this = getThis();
  zval retval = _array_deleteFromBack(this);
  RETURN_ZVAL(&retval, 1, 1);
}

PHP_METHOD(Deque, removeFront)
{
  ZEND_PARSE_PARAMETERS_START(0, 0)
  ZEND_PARSE_PARAMETERS_END();
  zval *this = getThis();
  zval retval = _array__deleteFromFront(this);
  RETURN_ZVAL(&retval, 1, 1);
}