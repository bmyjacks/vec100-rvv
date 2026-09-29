// Independent implementation of ICU4C 78.3 ucnvlat1.cpp:135-320.
// © 2016 and later: Unicode, Inc. and others.
// License & terms of use: http://www.unicode.org/copyright.html
// Copyright (C) 2000-2015, International Business Machines
// Corporation and others. All Rights Reserved.
#include "kernel.h"
#include <cstddef>
#include <cstdint>
#include <riscv_vector.h>

// A block with an invalid unit is still speculatively narrowed in upstream
// before the pointers are rolled back. Do the same, including the bytes beyond
// the reported target pointer. Aliasing requires the original scalar order.
static bool overlaps(const void *a, size_t an, const void *b, size_t bn) {
    auto x = reinterpret_cast<uintptr_t>(a);
    auto y = reinterpret_cast<uintptr_t>(b);
    return an && bn && x < y + bn && y < x + an;
}

#define EMIT_ONE() do { oredChars |= u = *source++; *target++ = (uint8_t)u; } while (0)

void _Latin1FromUnicodeWithOffsets_rvv(UConverterFromUnicodeArgs *pArgs,
                                        UErrorCode *pErrorCode) {
    UConverter *cnv=pArgs->converter;
    const char16_t *source=pArgs->source, *sourceLimit=pArgs->sourceLimit;
    uint8_t *target=(uint8_t *)pArgs->target, *oldTarget=target;
    int32_t *offsets=pArgs->offsets;
    char16_t max=cnv->sharedData==&_Latin1Data ? 0xff : 0x7f;
    UChar32 cp=cnv->fromUChar32;
    char16_t c;
    int32_t sourceIndex=cp==0 ? 0 : -1;
    int32_t length=(int32_t)(sourceLimit-source);
    int32_t targetCapacity=(int32_t)(pArgs->targetLimit-pArgs->target);
    if(length<targetCapacity) targetCapacity=length;
    if(cp!=0 && targetCapacity>0) goto getTrail;

    if(targetCapacity>=16) {
        int32_t count, loops;
        loops=count=targetCapacity>>4;
        const size_t inputBytes=static_cast<size_t>(length)*sizeof(char16_t);
        const size_t outputBytes=static_cast<size_t>(targetCapacity);
        const size_t offsetBytes=offsets ? outputBytes*sizeof(int32_t) : 0;
        bool alias=overlaps(source,inputBytes,target,outputBytes) ||
            (offsets && (overlaps(offsets,offsetBytes,source,inputBytes) ||
                         overlaps(offsets,offsetBytes,target,outputBytes)));
        do {
            bool bad=false;
            if(!alias) {
                // Process a complete ICU block, even when VLEN=128 (vl=8).
                // Never commit offsets for a block that must roll back.
                for(int done=0; done<16;) {
                    size_t vl=__riscv_vsetvl_e16m1(16-done);
                    auto v=__riscv_vle16_v_u16m1(
                        reinterpret_cast<const uint16_t *>(source+done),vl);
                    auto invalid=__riscv_vmsgtu_vx_u16m1_b16(v,max,vl);
                    bad |= __riscv_vfirst_m_b16(invalid,vl)>=0;
                    auto narrow=__riscv_vncvt_x_x_w_u8mf2(v,vl);
                    __riscv_vse8_v_u8mf2(target+done,narrow,vl);
                    done+=static_cast<int>(vl);
                }
                source+=16;
                target+=16;
            } else {
                char16_t u, oredChars;
                oredChars=u=*source++;
                *target++=(uint8_t)u;
                EMIT_ONE(); EMIT_ONE(); EMIT_ONE(); EMIT_ONE(); EMIT_ONE();
                EMIT_ONE(); EMIT_ONE(); EMIT_ONE(); EMIT_ONE(); EMIT_ONE();
                EMIT_ONE(); EMIT_ONE(); EMIT_ONE(); EMIT_ONE(); EMIT_ONE();
                bad=oredChars>max;
            }
            if(bad) {
                source-=16;
                target-=16;
                break;
            }
        } while(--count>0);
        count=loops-count;
        targetCapacity-=16*count;
        if(offsets!=nullptr) {
            oldTarget+=16*count;
            while(count>0) {
                for(int j=0;j<16;++j) *offsets++=sourceIndex++;
                --count;
            }
        }
    }

    c=0;
        while(targetCapacity>0 && (c=*source++)<=max) {
            *target++=(uint8_t)c;
            --targetCapacity;
        }
        if(c>max) {
            cp=c;
            if(!U_IS_SURROGATE(cp)) {
                /* callback(unassigned) */
            } else if(U_IS_SURROGATE_LEAD(cp)) {
getTrail:
                if(source<sourceLimit) {
                    char16_t trail=*source;
                    if(U16_IS_TRAIL(trail)) {
                        ++source;
                        cp=U16_GET_SUPPLEMENTARY(cp,trail);
                    }
                } else {
                    cnv->fromUChar32=cp;
                    goto noMoreInput;
                }
            }
            *pErrorCode=U_IS_SURROGATE(cp) ? U_ILLEGAL_CHAR_FOUND : U_INVALID_CHAR_FOUND;
            cnv->fromUChar32=cp;
        }
noMoreInput:
    if(offsets!=nullptr) {
        size_t count=target-oldTarget;
        while(count>0) {
            *offsets++=sourceIndex++;
            --count;
        }
    }
    if(U_SUCCESS(*pErrorCode) && source<sourceLimit &&
       target>=(uint8_t *)pArgs->targetLimit) {
        *pErrorCode=U_BUFFER_OVERFLOW_ERROR;
    }
    pArgs->source=source;
    pArgs->target=(char *)target;
    pArgs->offsets=offsets;
}
#undef EMIT_ONE
