/* vary extension for PHP */

#ifndef PHP_VARY_H
# define PHP_VARY_H

#define TRACE(fmt, ...) do { trace(__FILE__, __LINE__, __FUNCTION__, fmt, ##__VA_ARGS__); } while (0)

static inline void trace(const char *file, int line, const char* function, const char *fmt, ...) {
  fprintf(stderr, "%s(%s:%d) - ", function, file, line);
  va_list args;
  va_start(args, fmt);
  vfprintf(stderr, fmt, args);
  fprintf(stderr, "\n");
  va_end(args);
}

extern zend_module_entry vary_module_entry;
# define phpext_vary_ptr &vary_module_entry

# define PHP_VARY_VERSION "0.1.0"

# if defined(ZTS) && defined(COMPILE_DL_VARY)
ZEND_TSRMLS_CACHE_EXTERN()
# endif

#endif	/* PHP_VARY_H */

