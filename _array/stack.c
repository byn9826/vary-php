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
  zval _addToBack_name, _addToBack_retval;
  ZVAL_STRING(&_addToBack_name, "_addToBack");
  zval params[1];
  ZVAL_COPY(&params[0], _value);
  call_user_function(
    EG(function_table),
    getThis(),
    &_addToBack_name,
    &_addToBack_retval,
    1,
    params TSRMLS_CC
  );
  zval_ptr_dtor(&_addToBack_name);
  RETURN_BOOL(&_addToBack_retval);
}

PHP_METHOD(Stack, pop)
{
  int num_args = ZEND_CALL_NUM_ARGS(execute_data);
  if (num_args != 0) {
    warning_params_exact_zero("\\Vary\\Stack pop() method");
  }
  zval _deleteFromBack_name, _deleteFromBack_retval;
  ZVAL_STRING(&_deleteFromBack_name, "_deleteFromBack");
  call_user_function(
    EG(function_table),
    getThis(),
    &_deleteFromBack_name,
    &_deleteFromBack_retval,
    0,
    NULL TSRMLS_CC
  );
  zval_ptr_dtor(&_deleteFromBack_name);
  RETURN_ZVAL(&_deleteFromBack_retval, 1, 1);
}