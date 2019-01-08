/* vary extension for PHP */
#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "php.h"
#include "ext/standard/info.h"
#include "php_vary.h"

#define warning_params_max_one(msg) php_error_docref0(NULL, E_WARNING, "%s requires at most one parameter", msg)
#define warning_params_exact_zero(msg) php_error_docref0(NULL, E_WARNING, "%s requires no parameter", msg)
#define warning_params_exact_one(msg) php_error_docref0(NULL, E_WARNING, "%s requires one parameter", msg)

zend_class_entry *_array_handle;
zend_class_entry *stack_handle;

ZEND_BEGIN_ARG_INFO(arginfo_void, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_single, 0)
  ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_array, 0)
  ZEND_ARG_INFO(0, array)
ZEND_END_ARG_INFO()

PHP_METHOD(_array, __construct)
{
  zval _items, *param;
  ZEND_PARSE_PARAMETERS_START(0, 1)
    Z_PARAM_OPTIONAL
    Z_PARAM_ARRAY(param)
  ZEND_PARSE_PARAMETERS_END();
  int num_args = ZEND_CALL_NUM_ARGS(execute_data);
  if (num_args == 0) {
    array_init(&_items);
  } else {
    if (num_args > 1) {
      warning_params_max_one("\\Vary\\Stack");
    }
    ZVAL_COPY(&_items, param);
  }
  zend_update_property(
    _array_handle,
    getThis(),
    "_items",
    sizeof("_items") - 1,
    &_items TSRMLS_CC
  );
  zval_ptr_dtor(&_items);
  RETURN_TRUE;
}

PHP_METHOD(_array, size)
{
  if (zend_parse_parameters_none() == FAILURE) {
    warning_params_exact_zero("\\Vary\\Stack size() method");
  }
  zval *rv, *_items;
  _items = zend_read_property(
    _array_handle,
    getThis(),
    "_items",
    sizeof("_items") - 1,
    0,
    rv TSRMLS_CC
  );
  RETURN_LONG(zend_hash_num_elements(Z_ARRVAL_P(_items)));
}

PHP_METHOD(_array, _addToBack)
{
  zval *_value, *rv, *_items, value;
  ZEND_PARSE_PARAMETERS_START(1, 1)
    Z_PARAM_ZVAL(_value)
  ZEND_PARSE_PARAMETERS_END();
  _items = zend_read_property(
    _array_handle,
    getThis(),
    "_items",
    sizeof("_items") - 1,
    0,
    rv TSRMLS_CC
  );
  ZVAL_COPY(&value, _value);
  uint32_t items_size = zend_hash_num_elements(Z_ARRVAL_P(_items));
  if (zend_hash_next_index_insert(Z_ARRVAL_P(_items), &value) == NULL) {
    zval_ptr_dtor(&value);
    RETURN_FALSE;
  }
  zend_update_property(
    _array_handle,
    getThis(),
    "_items",
    sizeof("_items") - 1,
    _items TSRMLS_CC
  );
  zval_ptr_dtor(&value);
  RETURN_TRUE;
}

PHP_METHOD(_array, _deleteFromBack)
{
  zval *_items, *rv;
  _items = zend_read_property(
    _array_handle,
    getThis(),
    "_items",
    sizeof("_items") - 1,
    0,
    rv TSRMLS_CC
  );
  uint32_t items_size = zend_hash_num_elements(Z_ARRVAL_P(_items));
  if (items_size == 0) {
		RETURN_NULL();
	}
  Bucket *last_item = Z_ARRVAL_P(_items)->arData + items_size - 1;
  zval return_item;
  ZVAL_COPY(&return_item, &last_item->val);
  zend_hash_del_bucket(Z_ARRVAL_P(_items), last_item);
  zend_hash_internal_pointer_reset(Z_ARRVAL_P(_items));
  RETURN_ZVAL(&return_item, 0, 0);
}

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

const zend_function_entry _array_funcs[] = {
  PHP_ME(_array, __construct, arginfo_array, ZEND_ACC_PUBLIC)
  PHP_ME(_array, size, arginfo_void, ZEND_ACC_PUBLIC)
  PHP_ME(_array, _addToBack, arginfo_single, ZEND_ACC_PROTECTED)
  PHP_ME(_array, _deleteFromBack, arginfo_void, ZEND_ACC_PROTECTED)
	PHP_FE_END
};

const zend_function_entry stack_funcs[] = {
  PHP_ME(Stack, push, arginfo_single, ZEND_ACC_PUBLIC)
  PHP_ME(Stack, pop, arginfo_void, ZEND_ACC_PUBLIC)
	PHP_FE_END
};

PHP_MINIT_FUNCTION(vary)
{
	/*
   * Vary - Data Structures:
   * Stack(_array)
	 */
	zend_class_entry _array_ce;
  zend_class_entry stack_ce;
  INIT_NS_CLASS_ENTRY(_array_ce, "Vary", "_array", _array_funcs);
  _array_handle = zend_register_internal_class(&_array_ce TSRMLS_CC);
  INIT_NS_CLASS_ENTRY(stack_ce, "Vary", "Stack", stack_funcs);
  stack_handle = zend_register_internal_class_ex(&stack_ce, _array_handle);
  return SUCCESS;
}

PHP_RINIT_FUNCTION(vary)
{
#if defined(ZTS) && defined(COMPILE_DL_VARY)
	ZEND_TSRMLS_CACHE_UPDATE();
#endif
	return SUCCESS;
}

static const zend_function_entry vary_functions[] = {
	PHP_FE_END
};

PHP_MINFO_FUNCTION(vary)
{
	php_info_print_table_start();
  php_info_print_table_header(2, "Library Name", "Vary");
  php_info_print_table_header(2, "Author", "Baozier");
	php_info_print_table_end();
}

zend_module_entry vary_module_entry = {
	STANDARD_MODULE_HEADER,
	"vary",
	vary_functions,
	PHP_MINIT(vary),
	NULL,
	PHP_RINIT(vary),
	NULL,
	PHP_MINFO(vary),
	PHP_VARY_VERSION,
	STANDARD_MODULE_PROPERTIES
};

#ifdef COMPILE_DL_VARY
# ifdef ZTS
ZEND_TSRMLS_CACHE_DEFINE()
# endif
ZEND_GET_MODULE(vary)
#endif
