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

PHP_METHOD(Queue, dequeue)
{
  int num_args = ZEND_CALL_NUM_ARGS(execute_data);
  if (num_args != 0) {
    warning_params_exact_zero("\\Vary\\Queue dequeue() method");
  }
  zval _deleteFromFront_name, _deleteFromFront_retval;
  ZVAL_STRING(&_deleteFromFront_name, "_deleteFromFront");
  call_user_function(
    EG(function_table),
    getThis(),
    &_deleteFromFront_name,
    &_deleteFromFront_retval,
    0,
    NULL TSRMLS_CC
  );
  zval_ptr_dtor(&_deleteFromFront_name);
  RETURN_ZVAL(&_deleteFromFront_retval, 1, 1);
}