dnl $Id$
dnl config.m4 for extension postal

dnl Comments in this file start with the string 'dnl'.
dnl Remove where necessary. This file will not work
dnl without editing.

dnl If your extension references something external, use with:

PHP_ARG_WITH(libpostal-dir, for libpostal support,
[  --with-libpostal-dir[=DIR]             Set the path to libpostal install prefix.], yes)

if test "$PHP_POSTAL" != "no"; then
  dnl Write more examples of tests here...

  AC_PATH_PROG(PKG_CONFIG, pkg-config, no)
  if test "x$PKG_CONFIG" = "xno"; then
    AC_MSG_RESULT([pkg-config not found])
    AC_MSG_ERROR([Please reinstall pkg-config])
  fi

  if ! $PKG_CONFIG --exists libpostal; then
    AC_MSG_ERROR([php_postal support requires libpostal. Use --with-libpostal-dir=<DIR> to specify the prefix where libpostal headers and library are located])
  else
    LIBPOSTAL_VERSION=`$PKG_CONFIG libpostal --modversion`

    AC_MSG_RESULT([found version $LIBPOSTAL_VERSION])

    LIBPOSTAL_LIBS=`$PKG_CONFIG libpostal --libs`
    LIBPOSTAL_INCLUDES=`$PKG_CONFIG libpostal --cflags`

    PHP_EVAL_LIBLINE($LIBPOSTAL_LIBS, POSTAL_SHARED_LIBADD)
    PHP_EVAL_INCLINE($LIBPOSTAL_INCLUDES)

    PHP_SUBST(POSTAL_SHARED_LIBADD)

    PHP_NEW_EXTENSION(postal, php_postal.c, $ext_shared)
  fi
fi
