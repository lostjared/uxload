AC_PREREQ([2.64])
AC_INIT(uxload, 1.0, lostjared@lostsidedead.com)
AC_CONFIG_HEADERS(config.h)
AC_CANONICAL_SYSTEM
AM_INIT_AUTOMAKE
AM_PROG_AS
AC_LANG_CPLUSPLUS
AC_PROG_CXX
AC_PROG_LIBTOOL
AC_PROG_LN_S
AC_PROG_MAKE_SET

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_TYPE_PID_T
AC_FUNC_FORK

if test ${prefix} != "NONE"; then
LDFLAGS="$LDFLAGS -L${prefix}/lib"
fi

AC_OUTPUT(Makefile uxload/Makefile)
