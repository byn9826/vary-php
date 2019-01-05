/* vary extension for PHP */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "php.h"
#include "ext/standard/info.h"
#include "php_vary.h"

zend_class_entry *vary_stack_ce;

ZEND_BEGIN_ARG_INFO_EX(arginfo_void, 0, 0, 0)
ZEND_END_ARG_INFO()

PHP_METHOD(Stack, __construct)
{
  if (zend_parse_parameters_none() == FAILURE) { return; }
  zval _items;
  array_init(&_items);
  zend_update_property(
    vary_stack_ce,
    getThis(),
    "_items",
    sizeof("_items") - 1,
    &_items TSRMLS_CC
  );
  zval_ptr_dtor(&_items);
}

PHP_METHOD(Stack, size)
{
  if (zend_parse_parameters_none() == FAILURE) { return; }
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
  PHP_ME(Stack, __construct, arginfo_void, ZEND_ACC_PUBLIC)
  PHP_ME(Stack, size, arginfo_void, ZEND_ACC_PUBLIC)
  //PHP_ME(Stack, isEmpty, arginfo_void, ZEND_ACC_PUBLIC)
	PHP_FE_END
};

PHP_MINIT_FUNCTION(vary)
{
	/*
   * Vary Classes for Data Structures:
   * Stack
	 */
	zend_class_entry stack_ce;
  INIT_NS_CLASS_ENTRY(stack_ce, "Vary", "Stack", stack_functions);
  vary_stack_ce = zend_register_internal_class(&stack_ce TSRMLS_CC);
  // zval *stack_items = (zval *)malloc(sizeof(zval));
  // zend_declare_property(
  //   vary_stack_ce,
  //   "_items",
  //   sizeof("_items") - 1,
  //   stack_items,
  //   ZEND_ACC_PROTECTED TSRMLS_CC
  // );
  return SUCCESS;
}

PHP_RINIT_FUNCTION(vary)
{
#if defined(ZTS) && defined(COMPILE_DL_VARY)
	ZEND_TSRMLS_CACHE_UPDATE();
#endif
	return SUCCESS;
}

/*
 * Vary Helper functions:
 */
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
