/* vary extension for PHP */
#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "php.h"
#include "ext/standard/info.h"

#include "php_vary.h"
#include "./helpers/params.c"

#include "./_array/_array.c"
extern zend_class_entry *_array_handle;
const zend_function_entry _array_funcs[] = {
  PHP_ME(_array, __construct, arginfo_array, ZEND_ACC_PUBLIC)
  PHP_ME(_array, size, arginfo_void, ZEND_ACC_PUBLIC)
	PHP_ME(_array, _addToFront, arginfo_any, ZEND_ACC_PROTECTED)
  PHP_ME(_array, _addToBack, arginfo_any, ZEND_ACC_PROTECTED)
	PHP_ME(_array, _deleteFromFront, arginfo_void, ZEND_ACC_PROTECTED)
  PHP_ME(_array, _deleteFromBack, arginfo_void, ZEND_ACC_PROTECTED)
	PHP_FE_END
};

#include "./_array/stack.c"
extern zend_class_entry *stack_handle;
const zend_function_entry stack_funcs[] = {
  PHP_ME(Stack, push, arginfo_any, ZEND_ACC_PUBLIC)
  PHP_ME(Stack, pop, arginfo_void, ZEND_ACC_PUBLIC)
	PHP_FE_END
};

#include "./_array/queue.c"
extern zend_class_entry *queue_handle;
const zend_function_entry queue_funcs[] = {
  PHP_ME(Queue, enqueue, arginfo_any, ZEND_ACC_PUBLIC)
	PHP_ME(Queue, dequeue, arginfo_void, ZEND_ACC_PUBLIC)
	PHP_FE_END
};

#include "./_array/deque.c"
extern zend_class_entry *deque_handle;
const zend_function_entry deque_funcs[] = {
	PHP_ME(Deque, addFront, arginfo_any, ZEND_ACC_PUBLIC)
	PHP_ME(Deque, addRear, arginfo_any, ZEND_ACC_PUBLIC)
	PHP_ME(Deque, removeFront, arginfo_void, ZEND_ACC_PUBLIC)
  PHP_ME(Deque, removeRear, arginfo_void, ZEND_ACC_PUBLIC)
	PHP_FE_END
};

#include "./algorithm/algorithm.c"
extern zend_class_entry *algorithm_handle;
const zend_function_entry algorithm_funcs[] = {
	PHP_ME(Algorithm, binarySearch, arginfo_array_integer, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
	PHP_FE_END
};

PHP_MINIT_FUNCTION(vary)
{
	/*
	 * Data Structures Classes
	 * Parent: _array
	 * Childs: Stack, Queue, Deque
	 */
	zend_class_entry _array_ce;
  INIT_NS_CLASS_ENTRY(_array_ce, "Vary", "_array", _array_funcs);
  _array_handle = zend_register_internal_class(&_array_ce TSRMLS_CC);

  zend_class_entry stack_ce;
  INIT_NS_CLASS_ENTRY(stack_ce, "Vary", "Stack", stack_funcs);
  stack_handle = zend_register_internal_class_ex(&stack_ce, _array_handle);

	zend_class_entry queue_ce;
  INIT_NS_CLASS_ENTRY(queue_ce, "Vary", "Queue", queue_funcs);
  queue_handle = zend_register_internal_class_ex(&queue_ce, _array_handle);
  
	zend_class_entry deque_ce;
	INIT_NS_CLASS_ENTRY(deque_ce, "Vary", "Deque", deque_funcs);
	deque_handle = zend_register_internal_class_ex(&deque_ce, _array_handle);
  
	/*
	 * Algorithm Classes: Algorithm
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
