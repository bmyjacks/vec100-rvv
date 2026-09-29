/****************************************************************************
 *
 *
 *  Project: GLib 2.90.0
 *  Source files:
 *    glib/gtypes.h
 *    glib/glibconfig.h.in
 *    tools/gen-visibility-macros.py
 *    glib/gmacros.h
 *    glib/gstrfuncs.h
 *    glib/gmessages.h
 *
 *
 *  Below are the copyright notices of original files
 *
 *
 * glib/gtypes.h
 *
 *   GLIB - Library of useful routines for C programming
 *
 * Copyright (C) 1995-1997  Peter Mattis, Spencer Kimball and Josh MacDonald
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, see <http://www.gnu.org/licenses/>.
 *
 * Modified by the GLib Team and others 1997-2000.  See the AUTHORS
 * file for a list of people on the GLib Team.  See the ChangeLog
 * files for a list of changes.  These files are distributed with
 * GLib at ftp://ftp.gtk.org/pub/gtk/.
 *
 *
 * glib/glibconfig.h.in
 *
 *   This is a generated file.  Please modify 'glibconfig.h.in'
 *
 *
 * tools/gen-visibility-macros.py
 *
 * Copyright © 2022 Collabora Inc.
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 * Original author: Xavier Claessens <xclaesse@gmail.com>
 *
 * glib/gmacros.h
 *
 *   GLIB - Library of useful routines for C programming
 *
 * Copyright (C) 1995-1997  Peter Mattis, Spencer Kimball and Josh MacDonald
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, see <http://www.gnu.org/licenses/>.
 *
 * Modified by the GLib Team and others 1997-2000.  See the AUTHORS
 * file for a list of people on the GLib Team.  See the ChangeLog
 * files for a list of changes.  These files are distributed with
 * GLib at ftp://ftp.gtk.org/pub/gtk/.
 *
 *
 * glib/gstrfuncs.h
 *
 *   GLIB - Library of useful routines for C programming
 *
 * Copyright (C) 1995-1997  Peter Mattis, Spencer Kimball and Josh MacDonald
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, see <http://www.gnu.org/licenses/>.
 *
 * Modified by the GLib Team and others 1997-2000.  See the AUTHORS
 * file for a list of people on the GLib Team.  See the ChangeLog
 * files for a list of changes.  These files are distributed with
 * GLib at ftp://ftp.gtk.org/pub/gtk/.
 *
 *
 * glib/gmessages.h
 *
 *   GLIB - Library of useful routines for C programming
 *
 * Copyright (C) 1995-1997  Peter Mattis, Spencer Kimball and Josh MacDonald
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, see <http://www.gnu.org/licenses/>.
 *
 * Modified by the GLib Team and others 1997-2000.  See the AUTHORS
 * file for a list of people on the GLib Team.  See the ChangeLog
 * files for a list of changes.  These files are distributed with
 * GLib at ftp://ftp.gtk.org/pub/gtk/.
 *
 */

#ifndef KERNELS_28_G_ASCII_STRDOWN_INCLUDE_KERNEL_H_
#define KERNELS_28_G_ASCII_STRDOWN_INCLUDE_KERNEL_H_

/*
 * glib/glibconfig.h.in:87 (configured for unsigned long).
 */
#define G_MAXSIZE (~(gsize)0)

/*
 * glib/gtypes.h:52
 */
typedef char gchar;

/*
 * glib/gtypes.h:58
 */
typedef unsigned char guchar;

/*
 * glib/glibconfig.h.in:47 (configured as unsigned short)
 */
typedef unsigned short guint16;

/*
 * glib/glibconfig.h.in:80-81 (configured as signed/unsigned long)
 */
typedef signed long gssize;
typedef unsigned long gsize;

/*
 * tools/gen-visibility-macros.py:94-114
 */
#if (defined(_WIN32) || defined(__CYGWIN__)) &&                                \
    !defined(GLIB_STATIC_COMPILATION)
#define _GLIB_EXPORT __declspec(dllexport)
#define _GLIB_IMPORT __declspec(dllimport)
#elif __GNUC__ >= 4
#define _GLIB_EXPORT __attribute__((visibility("default")))
#define _GLIB_IMPORT
#else
#define _GLIB_EXPORT
#define _GLIB_IMPORT
#endif
#ifdef GLIB_COMPILATION
#define _GLIB_API _GLIB_EXPORT
#else
#define _GLIB_API _GLIB_IMPORT
#endif

#define _GLIB_EXTERN _GLIB_API extern

#define GLIB_VAR _GLIB_EXTERN
#define GLIB_AVAILABLE_IN_ALL _GLIB_EXTERN

/*
 * glib/gmacros.h:48-55
 */
#ifdef __GNUC__
#define G_GNUC_CHECK_VERSION(major, minor)                                     \
    ((__GNUC__ > (major)) ||                                                   \
     ((__GNUC__ == (major)) && (__GNUC_MINOR__ >= (minor))))
#else
#define G_GNUC_CHECK_VERSION(major, minor) 0
#endif

/*
 * glib/gmacros.h:61-65
 */
#if G_GNUC_CHECK_VERSION(2, 8)
#define G_GNUC_EXTENSION __extension__
#else
#define G_GNUC_EXTENSION
#endif

/*
 * glib/gmacros.h:67-112
 */
#if !defined(__cplusplus)

#undef G_CXX_STD_VERSION
#define G_CXX_STD_CHECK_VERSION(version) (0)

#if defined(__STDC_VERSION__)
#define G_C_STD_VERSION __STDC_VERSION__
#else
#define G_C_STD_VERSION 199000L
#endif

#define G_C_STD_CHECK_VERSION(version)                                         \
    (((version) >= 199000L && (version) <= G_C_STD_VERSION) ||                 \
     ((version) == 89 && G_C_STD_VERSION >= 199000L) ||                        \
     ((version) == 90 && G_C_STD_VERSION >= 199000L) ||                        \
     ((version) == 99 && G_C_STD_VERSION >= 199901L) ||                        \
     ((version) == 11 && G_C_STD_VERSION >= 201112L) ||                        \
     ((version) == 17 && G_C_STD_VERSION >= 201710L) ||                        \
     ((version) == 23 && G_C_STD_VERSION >= 202000L) || 0)

#else

#undef G_C_STD_VERSION
#define G_C_STD_CHECK_VERSION(version) (0)

#if defined(_MSVC_LANG)
#define G_CXX_STD_VERSION (_MSVC_LANG > __cplusplus ? _MSVC_LANG : __cplusplus)
#else
#define G_CXX_STD_VERSION __cplusplus
#endif

#define G_CXX_STD_CHECK_VERSION(version)                                       \
    (((version) >= 199711L && (version) <= G_CXX_STD_VERSION) ||               \
     ((version) == 98 && G_CXX_STD_VERSION >= 199711L) ||                      \
     ((version) == 03 && G_CXX_STD_VERSION >= 199711L) ||                      \
     ((version) == 11 && G_CXX_STD_VERSION >= 201103L) ||                      \
     ((version) == 14 && G_CXX_STD_VERSION >= 201402L) ||                      \
     ((version) == 17 && G_CXX_STD_VERSION >= 201703L) ||                      \
     ((version) == 20 && G_CXX_STD_VERSION >= 202002L) ||                      \
     ((version) == 23 && G_CXX_STD_VERSION >= 202302L) || 0)

#endif

/*
 * glib/gmacros.h:170-200
 */
#ifdef __has_attribute
#define g_macro__has_attribute __has_attribute
#else
#define g_macro__has_attribute(x) g_macro__has_attribute_##x

#define g_macro__has_attribute___alloc_size__ G_GNUC_CHECK_VERSION(4, 3)
#define g_macro__has_attribute___always_inline__ G_GNUC_CHECK_VERSION(2, 0)
#define g_macro__has_attribute___const__ G_GNUC_CHECK_VERSION(2, 4)
#define g_macro__has_attribute___deprecated__ G_GNUC_CHECK_VERSION(3, 1)
#define g_macro__has_attribute___format__ G_GNUC_CHECK_VERSION(2, 4)
#define g_macro__has_attribute___format_arg__ G_GNUC_CHECK_VERSION(2, 4)
#define g_macro__has_attribute___malloc__ G_GNUC_CHECK_VERSION(2, 96)
#define g_macro__has_attribute___no_instrument_function__                      \
    G_GNUC_CHECK_VERSION(2, 4)
#define g_macro__has_attribute___noinline__ G_GNUC_CHECK_VERSION(2, 96)
#define g_macro__has_attribute___noreturn__                                    \
    (G_GNUC_CHECK_VERSION(2, 8) || (0x5110 <= __SUNPRO_C))
#define g_macro__has_attribute___pure__ G_GNUC_CHECK_VERSION(2, 96)
#define g_macro__has_attribute___sentinel__ G_GNUC_CHECK_VERSION(4, 0)
#define g_macro__has_attribute___unused__ G_GNUC_CHECK_VERSION(2, 4)
#define g_macro__has_attribute___weak__ G_GNUC_CHECK_VERSION(2, 8)
#define g_macro__has_attribute_cleanup G_GNUC_CHECK_VERSION(3, 3)
#define g_macro__has_attribute_fallthrough G_GNUC_CHECK_VERSION(6, 0)
#define g_macro__has_attribute_may_alias G_GNUC_CHECK_VERSION(3, 3)
#define g_macro__has_attribute_warn_unused_result G_GNUC_CHECK_VERSION(3, 4)
#define g_macro__has_attribute_no_sanitize_address 0
#define g_macro__has_attribute_ifunc 0

#endif

/*
 * glib/gmacros.h:296-300
 */
#if g_macro__has_attribute(__malloc__)
#define G_GNUC_MALLOC __attribute__((__malloc__))
#else
#define G_GNUC_MALLOC
#endif

/*
 * glib/gmacros.h:345-349
 */
#ifdef __has_feature
#define g_macro__has_feature __has_feature
#else
#define g_macro__has_feature(x) 0
#endif

/*
 * glib/gmacros.h:639-644
 */
#if g_macro__has_attribute(__const__)
#define G_GNUC_CONST __attribute__((__const__))
#else
#define G_GNUC_CONST
#endif

/*
 * glib/gmacros.h:808-812
 */
#if g_macro__has_attribute(flag_enum)
#define G_GNUC_FLAG_ENUM __attribute__((flag_enum))
#else
#define G_GNUC_FLAG_ENUM
#endif

/*
 * glib/gmacros.h:888-897
 */
#if g_macro__has_feature(attribute_analyzer_noreturn) &&                       \
    defined(__clang_analyzer__)
#define G_ANALYZER_ANALYZING 1
#define G_ANALYZER_NORETURN __attribute__((analyzer_noreturn))
#elif defined(__COVERITY__)
#define G_ANALYZER_ANALYZING 1
#define G_ANALYZER_NORETURN __attribute__((noreturn))
#else
#define G_ANALYZER_ANALYZING 0
#define G_ANALYZER_NORETURN
#endif

/*
 * glib/gmacros.h:902-903
 */
#define G_PASTE_ARGS(identifier1, identifier2) identifier1##identifier2
#define G_PASTE(identifier1, identifier2) G_PASTE_ARGS(identifier1, identifier2)

/*
 * glib/gmacros.h:932-940
 */
#if defined(__GNUC__) && defined(G_CXX_STD_VERSION)
#define G_STRFUNC ((const char *)(__PRETTY_FUNCTION__))
#elif G_C_STD_CHECK_VERSION(99)
#define G_STRFUNC ((const char *)(__func__))
#elif defined(__GNUC__) || (defined(_MSC_VER) && (_MSC_VER > 1300))
#define G_STRFUNC ((const char *)(__FUNCTION__))
#else
#define G_STRFUNC ((const char *)("???"))
#endif

/*
 * glib/gmacros.h:1026-1037
 */
#if !(defined(G_STMT_START) && defined(G_STMT_END))
#define G_STMT_START do
#if defined(_MSC_VER) && (_MSC_VER >= 1500)
#define G_STMT_END                                                             \
    __pragma(warning(push)) __pragma(warning(disable : 4127)) while (0)        \
        __pragma(warning(pop))
#else
#define G_STMT_END while (0)
#endif
#endif

/*
 * glib/gmacros.h:1275-1289
 */
#if G_GNUC_CHECK_VERSION(2, 0) && defined(__OPTIMIZE__)
#define _G_BOOLEAN_EXPR_IMPL(uniq, expr)                                       \
    G_GNUC_EXTENSION({                                                         \
        int G_PASTE(_g_boolean_var_, uniq) = 0;                                \
        if (expr)                                                              \
            G_PASTE(_g_boolean_var_, uniq) = 1;                                \
        G_PASTE(_g_boolean_var_, uniq);                                        \
    })
#define _G_BOOLEAN_EXPR(expr) _G_BOOLEAN_EXPR_IMPL(__COUNTER__, expr)
#define G_LIKELY(expr) (__builtin_expect(_G_BOOLEAN_EXPR(expr), 1))
#define G_UNLIKELY(expr) (__builtin_expect(_G_BOOLEAN_EXPR(expr), 0))
#else
#define G_LIKELY(expr) (expr)
#define G_UNLIKELY(expr) (expr)
#endif

/*
 * glib/gstrfuncs.h:45-57
 */
typedef enum {
    G_ASCII_ALNUM = 1 << 0,
    G_ASCII_ALPHA = 1 << 1,
    G_ASCII_CNTRL = 1 << 2,
    G_ASCII_DIGIT = 1 << 3,
    G_ASCII_GRAPH = 1 << 4,
    G_ASCII_LOWER = 1 << 5,
    G_ASCII_PRINT = 1 << 6,
    G_ASCII_PUNCT = 1 << 7,
    G_ASCII_SPACE = 1 << 8,
    G_ASCII_UPPER = 1 << 9,
    G_ASCII_XDIGIT = 1 << 10
} G_GNUC_FLAG_ENUM GAsciiType;

/*
 * glib/gstrfuncs.h:59
 */
GLIB_VAR const guint16 *const g_ascii_table;

/*
 * glib/gstrfuncs.h:76-77
 */
#define g_ascii_islower(c) ((g_ascii_table[(guchar)(c)] & G_ASCII_LOWER) != 0)

/*
 * glib/gstrfuncs.h:88-89
 */
#define g_ascii_isupper(c) ((g_ascii_table[(guchar)(c)] & G_ASCII_UPPER) != 0)

/*
 * glib/gstrfuncs.h:94-97
 */
GLIB_AVAILABLE_IN_ALL
gchar g_ascii_tolower(gchar c) G_GNUC_CONST;
GLIB_AVAILABLE_IN_ALL
gchar g_ascii_toupper(gchar c) G_GNUC_CONST;

/*
 * glib/gstrfuncs.h:257-262
 */
GLIB_AVAILABLE_IN_ALL
gchar *g_ascii_strdown(const gchar *str, gssize len) G_GNUC_MALLOC;
GLIB_AVAILABLE_IN_ALL
gchar *g_ascii_strup(const gchar *str, gssize len) G_GNUC_MALLOC;

/*
 * glib/gstrfuncs.h:283-284
 */
GLIB_AVAILABLE_IN_ALL
gchar *(g_strdup)(const gchar *str)G_GNUC_MALLOC;

/*
 * glib/gstrfuncs.h:291-293
 */
GLIB_AVAILABLE_IN_ALL
gchar *g_strndup(const gchar *str, gsize n) G_GNUC_MALLOC;

/*
 * glib/gmessages.h:296-299
 */
GLIB_AVAILABLE_IN_ALL
void g_return_if_fail_warning(const char *log_domain,
                              const char *pretty_function,
                              const char *expression) G_ANALYZER_NORETURN;

/*
 * glib/gmessages.h:323-325
 */
#ifndef G_LOG_DOMAIN
#define G_LOG_DOMAIN ((gchar *)0)
#endif

/*
 * glib/gmessages.h:567-698
 */
#ifdef G_DISABLE_CHECKS

#define g_return_if_fail(expr)                                                 \
    G_STMT_START { (void)0; }                                                  \
    G_STMT_END

#define g_return_val_if_fail(expr, val)                                        \
    G_STMT_START { (void)0; }                                                  \
    G_STMT_END

#define g_return_if_reached()                                                  \
    G_STMT_START { return; }                                                   \
    G_STMT_END

#define g_return_val_if_reached(val)                                           \
    G_STMT_START { return (val); }                                             \
    G_STMT_END

#else

#define g_return_if_fail(expr)                                                 \
    G_STMT_START {                                                             \
        if (G_LIKELY(expr)) {                                                  \
        } else {                                                               \
            g_return_if_fail_warning(G_LOG_DOMAIN, G_STRFUNC, #expr);          \
            return;                                                            \
        }                                                                      \
    }                                                                          \
    G_STMT_END

#define g_return_val_if_fail(expr, val)                                        \
    G_STMT_START {                                                             \
        if (G_LIKELY(expr)) {                                                  \
        } else {                                                               \
            g_return_if_fail_warning(G_LOG_DOMAIN, G_STRFUNC, #expr);          \
            return (val);                                                      \
        }                                                                      \
    }                                                                          \
    G_STMT_END

#define g_return_if_reached()                                                  \
    G_STMT_START {                                                             \
        g_log(G_LOG_DOMAIN, G_LOG_LEVEL_CRITICAL,                              \
              "file %s: line %d (%s): should not be reached", __FILE__,        \
              __LINE__, G_STRFUNC);                                            \
        return;                                                                \
    }                                                                          \
    G_STMT_END

#define g_return_val_if_reached(val)                                           \
    G_STMT_START {                                                             \
        g_log(G_LOG_DOMAIN, G_LOG_LEVEL_CRITICAL,                              \
              "file %s: line %d (%s): should not be reached", __FILE__,        \
              __LINE__, G_STRFUNC);                                            \
        return (val);                                                          \
    }                                                                          \
    G_STMT_END

#endif

#endif // KERNELS_28_G_ASCII_STRDOWN_INCLUDE_KERNEL_H_
