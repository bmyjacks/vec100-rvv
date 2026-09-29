/****************************************************************************
 *
 *
 *  Project: SQLite 3.53.4
 *  Source files:
 *    ext/misc/base85.c
 *
 *
 *  The original file copyright and license notices follow.
 *
 *
 * ext/misc/base85.c
 *
 * 2022-11-16
 *
 * The author disclaims copyright to this source code.  In place of
 * a legal notice, here is a blessing:
 *
 *    May you do good and not evil.
 *    May you find forgiveness for yourself and forgive others.
 *    May you share freely, never taking more than you give.
 *
 * This is a utility for converting binary to base85 or vice-versa.
 * It can be built as a standalone program or an SQLite3 extension.
 *
 */

#ifndef KERNELS_80_SQLITE_TO_BASE85_INCLUDE_KERNEL_H_
#define KERNELS_80_SQLITE_TO_BASE85_INCLUDE_KERNEL_H_

/*
 * Wrapper for invoking the extracted kernel. The input has nbIn readable
 * bytes (nbIn >= 0); pOut has room for encoded text, separators and NUL.
 * pSep is null or a NUL-terminated string. Returns the address of the NUL.
 */
char *sqlite_to_base85(unsigned char *pIn, int nbIn, char *pOut, char *pSep);

/*
 * RVV equivalent of the extracted kernel, with the same contract.
 */
char *sqlite_to_base85_rvv(unsigned char *pIn, int nbIn, char *pOut,
                           char *pSep);

#endif // KERNELS_80_SQLITE_TO_BASE85_INCLUDE_KERNEL_H_
