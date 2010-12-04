# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.61])
AC_INIT(spatosc, [0.1], [https://github.com/mikewoz/spatosc])
AC_MSG_NOTICE([Configuring spatosc])
AM_INIT_AUTOMAKE([subdir-objects -Wno-portability])
AC_CONFIG_HEADERS([include/config.h])
AC_CANONICAL_HOST # fills $host variable

# Special cases
case "$host" in *-darwin* | *-macos10*)
  CPPFLAGS="$CPPFLAGS -D__Darwin"
  LDFLAGS="-undefined dynamic_lookup -Wl,-headerpad_max_install_names"
  if test -d /opt/local ; then
    CPPFLAGS="$CPPFLAGS -I/usr/local/include -I/opt/local/include"
    LDFLAGS="$LDFLAGS -L/usr/local/lib -L/opt/local/lib"
  elif test -d /sw ; then
    CPPFLAGS="$CPPFLAGS -I/sw/include"
    LDFLAGS="$LDFLAGS -L/sw/lib"
  fi
  ;;
esac

# OSX does not include /usr/local/lib/pkgconfig path, so we export it here
# (for everyone, just in case)
PKG_CONFIG_PATH=/usr/local/lib/pkgconfig:$PKG_CONFIG_PATH
export PKG_CONFIG_PATH

#CXXFLAGS+=" -g3 -ggdb -Wall -Wextra -Wfatal-errors"
CXXFLAGS+=" -Wall -Wextra -Wfatal-errors"
LDFLAGS="$LDFLAGS -L/usr/local/lib -L/usr/local/lib64"

AC_CONFIG_FILES([
        Makefile 
        src/Makefile 
        ])

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AC_PROG_INSTALL
AC_PROG_LIBTOOL

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_INLINE
AC_TYPE_SIZE_T
AC_CHECK_TYPES([ptrdiff_t])


# Checks for libraries
AC_CHECK_LIB([m],[sqrt])

# Checks for header files.
AC_CHECK_HEADERS([arpa/inet.h stddef.h stdlib.h string.h sys/time.h])

# Checks for library functions.
AC_FUNC_ERROR_AT_LINE
AC_CHECK_FUNCS([gettimeofday memmove mkdir setenv sqrt strchr strpbrk uname])

# check for liblo:
AC_CHECK_LIB(lo, lo_send_internal, LIBLO_LDFLAGS="-llo", AC_MSG_ERROR([missing liblo OSC library]),)
AC_SUBST([LIBLO_LDFLAGS])

# GNU help2man creates man pages from --help output; in many cases, this
# is sufficient, and obviates the need to maintain man pages separately.
# However, this means invoking executables, which we generally cannot do
# when cross-compiling, so we test to avoid that (the variable
# "cross_compiling" is set by AC_PROG_CC).
if test $cross_compiling = no; then
    AM_MISSING_PROG(HELP2MAN, help2man)
else
    HELP2MAN=:
fi

AC_OUTPUT
