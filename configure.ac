# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.61])
AC_INIT(spatosc, [0.1.0], [http://github.com/mikewoz/spatosc/issues], [spatosc], [http://github.com/mikewoz/spatosc])
AC_CONFIG_MACRO_DIR([m4])
AC_MSG_NOTICE([Configuring spatosc])
AM_INIT_AUTOMAKE([subdir-objects -Wno-portability])
AC_CONFIG_HEADERS([include/config.h])
AC_CANONICAL_HOST # fills $host variable
# AC_CONFIG_MACRO_DIR([m4])

# Define these substitions here to keep all version information in one place.
# For information on how to properly maintain the library version information,
# refer to the libtool manual, section "Updating library version information":
# http://www.gnu.org/software/libtool/manual/html_node/Updating-version-info.html
# The version of the API. Should be major.minor and not change until not backward-compatible
AC_SUBST([SPATOSC_SO_VERSION], [0:1:0])
AC_SUBST([SPATOSC_API_VERSION], [0.1])

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

ACX_PTHREAD

# Beware! From now on, we might modify the CXXFLAGS and LDFLAGS.
# --------------------------------------------------------------

AC_TRY_COMPILE([#include <sys/types.h>
        #include <regex.h>],[  int i; 
        regex_t r;
        int cflags = REG_EXTENDED;
        const char *s = "abc";
        i = regcomp(&r,s,cflags);
        regfree(&r);
        ]; , have_regex=yes, have_regex=no)
if test "$have_regex" = yes; then
    AC_DEFINE(HAVE_REGEX,1,[Defined if found regex support. ])
    CXXFLAGS+=" -DHAVE_REGEX"
fi

# Special cases
case "$host" in *-darwin* | *-macos10*)
  CXXFLAGS+=" -D__Darwin"
  LDFLAGS+=" -undefined dynamic_lookup -Wl,-headerpad_max_install_names"
  if test -d /opt/local ; then
    CXXFLAGS+=" -I/usr/local/include -I/opt/local/include"
    LDFLAGS+=" -L/usr/local/lib -L/opt/local/lib"
  elif test -d /sw ; then
    CXXFLAGS+=" -I/sw/include"
    LDFLAGS+=" -L/sw/lib"
  fi
  ;;
esac

#TODO: is this going to mess up with GNU/Linux systems?
# OSX does not include /usr/local/lib/pkgconfig path, so we export it here
# (for everyone, just in case)
PKG_CONFIG_PATH=/usr/local/lib/pkgconfig:$PKG_CONFIG_PATH
export PKG_CONFIG_PATH

#CXXFLAGS+=" -g3 -ggdb -Wall -Wextra -Wfatal-errors"
CXXFLAGS+=" -Wall -Wextra -Wfatal-errors -Werror -Wnon-virtual-dtor"
# FIXME:2011-01-26: That one is for Mac OSX only, right?
LDFLAGS+=" -fPIC -L/usr/local/lib -L/usr/local/lib64"

#Set LIBLO with pkg-config
PKG_CHECK_MODULES(LIBLO, liblo, have_liblo=true, have_liblo=false)
if test "x${have_liblo}" = "xfalse" ; then
    AC_MSG_ERROR([Missing liblo])
fi

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

# Override the template file name of the generated .pc file, so that there
# is no need to rename the template file when the API version changes.
AC_CONFIG_FILES([
        Makefile 
        spatosc.pc 
        include/version.h
        src/Makefile 
        tests/Makefile
        doxyfile 
        spatosc-${SPATOSC_API_VERSION}.pc:spatosc.pc.in
        build/Makefile
        ])

AC_OUTPUT
