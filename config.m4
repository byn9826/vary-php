dnl config.m4 for extension vary

dnl Comments in this file start with the string 'dnl'.
dnl Remove where necessary.

dnl If your extension references something external, use with:

dnl PHP_ARG_WITH(vary, for vary support,
dnl Make sure that the comment is aligned:
dnl [  --with-vary             Include vary support])

dnl Otherwise use enable:

PHP_ARG_ENABLE(vary, whether to enable vary support,
dnl Make sure that the comment is aligned:
[  --enable-vary          Enable vary support], no)

if test "$PHP_VARY" != "no"; then
  dnl Write more examples of tests here...

  dnl # get library FOO build options from pkg-config output
  dnl AC_PATH_PROG(PKG_CONFIG, pkg-config, no)
  dnl AC_MSG_CHECKING(for libfoo)
  dnl if test -x "$PKG_CONFIG" && $PKG_CONFIG --exists foo; then
  dnl   if $PKG_CONFIG foo --atleast-version 1.2.3; then
  dnl     LIBFOO_CFLAGS=\`$PKG_CONFIG foo --cflags\`
  dnl     LIBFOO_LIBDIR=\`$PKG_CONFIG foo --libs\`
  dnl     LIBFOO_VERSON=\`$PKG_CONFIG foo --modversion\`
  dnl     AC_MSG_RESULT(from pkgconfig: version $LIBFOO_VERSON)
  dnl   else
  dnl     AC_MSG_ERROR(system libfoo is too old: version 1.2.3 required)
  dnl   fi
  dnl else
  dnl   AC_MSG_ERROR(pkg-config not found)
  dnl fi
  dnl PHP_EVAL_LIBLINE($LIBFOO_LIBDIR, VARY_SHARED_LIBADD)
  dnl PHP_EVAL_INCLINE($LIBFOO_CFLAGS)

  dnl # --with-vary -> check with-path
  dnl SEARCH_PATH="/usr/local /usr"     # you might want to change this
  dnl SEARCH_FOR="/include/vary.h"  # you most likely want to change this
  dnl if test -r $PHP_VARY/$SEARCH_FOR; then # path given as parameter
  dnl   VARY_DIR=$PHP_VARY
  dnl else # search default path list
  dnl   AC_MSG_CHECKING([for vary files in default path])
  dnl   for i in $SEARCH_PATH ; do
  dnl     if test -r $i/$SEARCH_FOR; then
  dnl       VARY_DIR=$i
  dnl       AC_MSG_RESULT(found in $i)
  dnl     fi
  dnl   done
  dnl fi
  dnl
  dnl if test -z "$VARY_DIR"; then
  dnl   AC_MSG_RESULT([not found])
  dnl   AC_MSG_ERROR([Please reinstall the vary distribution])
  dnl fi

  dnl # --with-vary -> add include path
  dnl PHP_ADD_INCLUDE($VARY_DIR/include)

  dnl # --with-vary -> check for lib and symbol presence
  dnl LIBNAME=VARY # you may want to change this
  dnl LIBSYMBOL=VARY # you most likely want to change this

  dnl PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
  dnl [
  dnl   PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $VARY_DIR/$PHP_LIBDIR, VARY_SHARED_LIBADD)
  dnl   AC_DEFINE(HAVE_VARYLIB,1,[ ])
  dnl ],[
  dnl   AC_MSG_ERROR([wrong vary lib version or lib not found])
  dnl ],[
  dnl   -L$VARY_DIR/$PHP_LIBDIR -lm
  dnl ])
  dnl
  dnl PHP_SUBST(VARY_SHARED_LIBADD)

  dnl # In case of no dependencies
  AC_DEFINE(HAVE_VARY, 1, [ Have vary support ])

  PHP_NEW_EXTENSION(vary, vary.c, $ext_shared)
fi
