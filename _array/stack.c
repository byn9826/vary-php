#include "php.h"

zend_class_entry *stack_handle;

PHP_METHOD(Stack, push)
{
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
  ZEND_PARSE_PARAMETERS_START(0, 0)
  ZEND_PARSE_PARAMETERS_END();
  zval *this = getThis();
  zval retval = _array_deleteFromBack(this);
  RETURN_ZVAL(&retval, 1, 1);
}