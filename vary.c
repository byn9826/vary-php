/* vary extension for PHP */
#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "php.h"
#include "ext/standard/info.h"
#include "php_vary.h"

extern zend_class_entry *algorithm_handle;
extern zend_class_entry *_array_handle;
extern zend_class_entry *arrayList_handle;
extern zend_class_entry *stack_handle;
extern zend_class_entry *queue_handle;
extern zend_class_entry *deque_handle;
extern zend_class_entry *orderedList_handle;

#include "./helpers/params.c"

#include "./others/Algorithm.c"
const zend_function_entry algorithm_funcs[] = {
	PHP_ME(Algorithm, binarySearch, arginfo_array_integer, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
	PHP_ME(Algorithm, shellSort, arginfo_array_function, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
	PHP_FE_END
};

#include "./lists/_array.c"
const zend_function_entry _array_funcs[] = {
  PHP_ME(_array, __construct, arginfo_array, ZEND_ACC_PUBLIC)
	PHP_ME(_array, value, arginfo_void, ZEND_ACC_PUBLIC)
  PHP_ME(_array, length, arginfo_void, ZEND_ACC_PUBLIC)
	PHP_ME(_array, index, arginfo_integer, ZEND_ACC_PUBLIC)
	PHP_ME(_array, setValue, arginfo_array, ZEND_ACC_PROTECTED)
	PHP_ME(_array, removeIndex, arginfo_integer, ZEND_ACC_PROTECTED)
	PHP_ME(_array, push, arginfo_any, ZEND_ACC_PROTECTED)
	PHP_ME(_array, pop, arginfo_void, ZEND_ACC_PROTECTED)
	PHP_ME(_array, unshift, arginfo_any, ZEND_ACC_PROTECTED)
	PHP_ME(_array, shift, arginfo_void, ZEND_ACC_PROTECTED)
	PHP_ME(_array, indexOf, arginfo_any, ZEND_ACC_PROTECTED)
	PHP_ME(_array, lastIndexOf, arginfo_any, ZEND_ACC_PROTECTED)
	PHP_ME(_array, includes, arginfo_any, ZEND_ACC_PROTECTED)
	PHP_ME(_array, sort, arginfo_function, ZEND_ACC_PROTECTED)
	PHP_ME(_array, reverse, arginfo_void, ZEND_ACC_PROTECTED)
	PHP_ME(_array, concat, arginfo_array, ZEND_ACC_PROTECTED)
	PHP_ME(_array, every, arginfo_function, ZEND_ACC_PROTECTED)
	PHP_ME(_array, some, arginfo_function, ZEND_ACC_PROTECTED)
	PHP_ME(_array, map, arginfo_function, ZEND_ACC_PROTECTED)
	PHP_ME(_array, forEach, arginfo_function, ZEND_ACC_PROTECTED)
	PHP_ME(_array, reduce, arginfo_function_any, ZEND_ACC_PROTECTED)
	PHP_ME(_array, filter, arginfo_function, ZEND_ACC_PROTECTED)
	PHP_ME(_array, find, arginfo_function, ZEND_ACC_PROTECTED)
	PHP_ME(_array, findIndex, arginfo_function, ZEND_ACC_PROTECTED)
	PHP_FE_END
};

const zend_function_entry arrayList_funcs[] = {
	PHP_ME(_array, setValue, arginfo_array, ZEND_ACC_PUBLIC)
	PHP_ME(_array, removeIndex, arginfo_integer, ZEND_ACC_PUBLIC)
	PHP_ME(_array, push, arginfo_any, ZEND_ACC_PUBLIC)
	PHP_ME(_array, pop, arginfo_void, ZEND_ACC_PUBLIC)
	PHP_ME(_array, unshift, arginfo_any, ZEND_ACC_PUBLIC)
	PHP_ME(_array, shift, arginfo_void, ZEND_ACC_PUBLIC)
	PHP_ME(_array, indexOf, arginfo_any, ZEND_ACC_PUBLIC)
	PHP_ME(_array, lastIndexOf, arginfo_any, ZEND_ACC_PUBLIC)
	PHP_ME(_array, includes, arginfo_any, ZEND_ACC_PUBLIC)
	PHP_ME(_array, sort, arginfo_function, ZEND_ACC_PUBLIC)
	PHP_ME(_array, reverse, arginfo_void, ZEND_ACC_PUBLIC)
	PHP_ME(_array, concat, arginfo_array, ZEND_ACC_PUBLIC)
	PHP_ME(_array, every, arginfo_function, ZEND_ACC_PUBLIC)
	PHP_ME(_array, some, arginfo_function, ZEND_ACC_PUBLIC)
	PHP_ME(_array, map, arginfo_function, ZEND_ACC_PUBLIC)
	PHP_ME(_array, forEach, arginfo_function, ZEND_ACC_PUBLIC)
	PHP_ME(_array, reduce, arginfo_function_any, ZEND_ACC_PUBLIC)
	PHP_ME(_array, filter, arginfo_function, ZEND_ACC_PUBLIC)
	PHP_ME(_array, find, arginfo_function, ZEND_ACC_PUBLIC)
	PHP_ME(_array, findIndex, arginfo_function, ZEND_ACC_PUBLIC)
	PHP_FE_END
};

const zend_function_entry stack_funcs[] = {
  PHP_ME(_array, push, arginfo_any, ZEND_ACC_PUBLIC)
  PHP_ME(_array, pop, arginfo_void, ZEND_ACC_PUBLIC)
	PHP_FE_END
};

const zend_function_entry queue_funcs[] = {
  PHP_ME(_array, push, arginfo_any, ZEND_ACC_PUBLIC)
	PHP_ME(_array, shift, arginfo_void, ZEND_ACC_PUBLIC)
	PHP_FE_END
};

const zend_function_entry deque_funcs[] = {
	PHP_ME(_array, push, arginfo_any, ZEND_ACC_PUBLIC)
	PHP_ME(_array, pop, arginfo_void, ZEND_ACC_PUBLIC)
	PHP_ME(_array, unshift, arginfo_any, ZEND_ACC_PUBLIC)
	PHP_ME(_array, shift, arginfo_void, ZEND_ACC_PUBLIC)
	PHP_FE_END
};

const zend_function_entry orderedList_funcs[] = {
	PHP_ME(OrderedList, __construct, arginfo_void, ZEND_ACC_PUBLIC)
	PHP_ME(_array, removeIndex, arginfo_integer, ZEND_ACC_PUBLIC)
	PHP_ME(_array, pop, arginfo_void, ZEND_ACC_PUBLIC)
	PHP_ME(_array, shift, arginfo_void, ZEND_ACC_PUBLIC)
	PHP_ME(OrderedList, add, arginfo_integer, ZEND_ACC_PUBLIC)
	PHP_ME(OrderedList, remove, arginfo_integer, ZEND_ACC_PUBLIC)
	PHP_ME(OrderedList, indexOf, arginfo_integer, ZEND_ACC_PUBLIC)
	PHP_FE_END
};

PHP_MINIT_FUNCTION(vary)
{
	/*
	 * _array Classes
	 * ArrayList, Stack, Queue, Deque, OrderedList
	 */
	zend_class_entry _array_ce;
  INIT_NS_CLASS_ENTRY(_array_ce, "Vary", "_array", _array_funcs);
  _array_handle = zend_register_internal_class(&_array_ce TSRMLS_CC);

	zend_class_entry arrayList_ce;
  INIT_NS_CLASS_ENTRY(arrayList_ce, "Vary", "ArrayList", arrayList_funcs);
  arrayList_handle = zend_register_internal_class_ex(&arrayList_ce, _array_handle);

  zend_class_entry stack_ce;
  INIT_NS_CLASS_ENTRY(stack_ce, "Vary", "Stack", stack_funcs);
  stack_handle = zend_register_internal_class_ex(&stack_ce, _array_handle);

	zend_class_entry queue_ce;
  INIT_NS_CLASS_ENTRY(queue_ce, "Vary", "Queue", queue_funcs);
  queue_handle = zend_register_internal_class_ex(&queue_ce, _array_handle);
  
	zend_class_entry deque_ce;
	INIT_NS_CLASS_ENTRY(deque_ce, "Vary", "Deque", deque_funcs);
	deque_handle = zend_register_internal_class_ex(&deque_ce, _array_handle);
  
	zend_class_entry orderedList_ce;
	INIT_NS_CLASS_ENTRY(orderedList_ce, "Vary", "OrderedList", orderedList_funcs);
  orderedList_handle = zend_register_internal_class_ex(&orderedList_ce, _array_handle);

	/*
	 * Algorithm Functions
	 * binarySearch, shellShort
	 */
	zend_class_entry algorithm_ce;
  INIT_NS_CLASS_ENTRY(algorithm_ce, "Vary", "Algorithm", algorithm_funcs);
  algorithm_handle = zend_register_internal_class(&algorithm_ce TSRMLS_CC);

	return SUCCESS;
}

PHP_RINIT_FUNCTION(vary)
{
#if defined(ZTS) && defined(COMPILE_DL_VARY)
	ZEND_TSRMLS_CACHE_UPDATE();
#endif
	return SUCCESS;
}

static const zend_function_entry vary_funcs[] = {
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
	vary_funcs,
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
