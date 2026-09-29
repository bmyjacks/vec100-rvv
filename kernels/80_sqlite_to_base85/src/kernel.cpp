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

#include "kernel.h"

/*
 * ext/misc/base85.c:114-116
 */
typedef unsigned char u8;

/*
 * ext/misc/base85.c:139-140
 */
#define B85_DARK_MAX 80

/*
 * ext/misc/base85.c:155-158
 */
#define base85Numeral(dn)                                                      \
    ((char)(((dn) < 4) ? (char)((dn) + '#') : (char)((dn) - 4 + '*')))

/*
 * ext/misc/base85.c:160-164
 */
static char *putcs(char *pc, char *s) {
    char c;
    while ((c = *s++) != 0)
        *pc++ = c;
    return pc;
}

/*
 * ext/misc/base85.c:170-208
 */
static char *toBase85(u8 *pIn, int nbIn, char *pOut, char *pSep) {
    int nCol = 0;
    while (nbIn >= 4) {
        int nco = 5;
        unsigned long qbv = (((unsigned long)pIn[0]) << 24) | (pIn[1] << 16) |
                            (pIn[2] << 8) | pIn[3];
        while (nco > 0) {
            unsigned nqv = (unsigned)(qbv / 85UL);
            unsigned char dv = qbv - 85UL * nqv;
            qbv = nqv;
            pOut[--nco] = base85Numeral(dv);
        }
        nbIn -= 4;
        pIn += 4;
        pOut += 5;
        if (pSep && (nCol += 5) >= B85_DARK_MAX) {
            pOut = putcs(pOut, pSep);
            nCol = 0;
        }
    }
    if (nbIn > 0) {
        int nco = nbIn + 1;
        unsigned long qv = *pIn++;
        int nbe = 1;
        while (nbe++ < nbIn) {
            qv = (qv << 8) | *pIn++;
        }
        nCol += nco;
        while (nco > 0) {
            u8 dv = (u8)(qv % 85);
            qv /= 85;
            pOut[--nco] = base85Numeral(dv);
        }
        pOut += (nbIn + 1);
    }
    if (pSep && nCol > 0)
        pOut = putcs(pOut, pSep);
    *pOut = 0;
    return pOut;
}

/*
 * Wrapper for invoking the extracted kernel.
 */
char *sqlite_to_base85(unsigned char *pIn, int nbIn, char *pOut, char *pSep) {
    return toBase85(pIn, nbIn, pOut, pSep);
}
