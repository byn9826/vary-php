/* vary extension for PHP */
#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "php.h"
#include "ext/standard/info.h"
#include "php_vary.h"

zend_class_entry *vary_stack_ce;

ZEND_BEGIN_ARG_INFO(arginfo_void, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_array_init, 0)
  ZEND_ARG_INFO(0, array)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_any, 0)
  ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()

PHP_METHOD(Stack, __construct)
{
  zval _items, *param;
  ZEND_PARSE_PARAMETERS_START(0, 1)
    Z_PARAM_OPTIONAL
    Z_PARAM_ARRAY(param)
  ZEND_PARSE_PARAMETERS_END();
  int num_args = ZEND_CALL_NUM_ARGS(execute_data);
  if (num_args > 1) {
    php_error_docref(
      NULL,
      E_WARNING,
      "\\Vary\\Stack only use the first parameter"
    );
    ZVAL_COPY(&_items, param);
  } else if (num_args == 1) {
    ZVAL_COPY(&_items, param);
  } else if (num_args == 0) {
    array_init(&_items);
  }
  zend_update_property(
    vary_stack_ce,
    getThis(),
    "_items",
    sizeof("_items") - 1,
    &_items TSRMLS_CC
  );
  zval_ptr_dtor(&_items);
  RETURN_TRUE;
}

PHP_METHOD(Stack, size)
{
  if (zend_parse_parameters_none() == FAILURE) { RETURN_FALSE; }
  zval *rv, *_items;
  _items = zend_read_property(
    vary_stack_ce,
    getThis(),
    "_items",
    sizeof("_items") - 1,
    0,
    rv TSRMLS_CC
  );
  RETURN_LONG(zend_hash_num_elements(Z_ARRVAL_P(_items)));
}

PHP_METHOD(Stack, push)
{
  zval *_value, *rv, *_items, value;
  ZEND_PARSE_PARAMETERS_START(1, 1)
    Z_PARAM_ZVAL(_value)
  ZEND_PARSE_PARAMETERS_END();
  _items = zend_read_property(
    vary_stack_ce,
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
    vary_stack_ce,
    getThis(),
    "_items",
    sizeof("_items") - 1,
    _items TSRMLS_CC
  );
  RETURN_TRUE;
}

PHP_METHOD(Stack, pop)
{
  int num_args = ZEND_CALL_NUM_ARGS(execute_data);
  if (num_args != 0) {
    php_error_docref(
      NULL,
      E_WARNING,
      "\\Vary\\Stack do not require any parameters"
    );
  }
  zval *_items, *rv;
  _items = zend_read_property(
    vary_stack_ce,
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
  RETURN_ZVAL(&return_item, 0, 1);
}

// PHP_METHOD(Stack, isEmpty)
// {
//   if (zend_parse_parameters_none() == FAILURE) { return; }
//   zval method_size_name, method_size_retval;
//   ZVAL_STR(&method_size_name, zend_string_init("size", sizeof("size") - 1, 0));
//   call_user_function(
//     &EG(function_table),
//     getThis(),
//     &method_size_name,
//     &method_size_retval,
//     0,
//     NULL TSRMLS_CC
//   );
//   long items_size = zval_get_long(&method_size_retval);
//   if (items_size == 0) {
//     RETURN_TRUE;
//   } else {
//     RETURN_FALSE;
//   }
// }

const zend_function_entry stack_functions[] = {
  PHP_ME(Stack, __construct, arginfo_array_init, ZEND_ACC_PUBLIC)
  PHP_ME(Stack, size, arginfo_void, ZEND_ACC_PUBLIC)
  PHP_ME(Stack, push, arginfo_any, ZEND_ACC_PUBLIC)
  PHP_ME(Stack, pop, arginfo_void, ZEND_ACC_PUBLIC)
	PHP_FE_END
};

PHP_MINIT_FUNCTION(vary)
{
	/*
   * Vary - Data Structures:
   * Stack
	 */
	zend_class_entry stack_ce;
  INIT_NS_CLASS_ENTRY(stack_ce, "Vary", "Stack", stack_functions);
  vary_stack_ce = zend_register_internal_class(&stack_ce TSRMLS_CC);
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
	php_info_print_table_header(2, "vary support", "enabled");
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
