#include "kernel.h"
#include <algorithm>
#include <array>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <initializer_list>
#include <random>

using Fn=void (*)(UConverterFromUnicodeArgs *, UErrorCode *);

static void expect(Fn fn, std::initializer_list<char16_t> units, int cap,
                   UChar32 pending, UErrorCode error, int consumed,
                   int produced, UChar32 state, int firstOffset=-2) {
    std::array<char16_t,32> src{};
    std::copy(units.begin(),units.end(),src.begin());
    std::array<char,32> dst{};
    std::array<int32_t,32> off{};
    dst.fill(char(0x5a)); off.fill(999);
    UConverter cnv{&_Latin1Data,pending};
    UConverterFromUnicodeArgs args{&cnv,src.data(),src.data()+units.size(),
                                    dst.data(),dst.data()+cap,off.data()};
    UErrorCode ec=U_ZERO_ERROR;
    fn(&args,&ec);
    if(ec!=error || args.source-src.data()!=consumed ||
       args.target-dst.data()!=produced || cnv.fromUChar32!=state ||
       args.offsets-off.data()!=produced ||
       (firstOffset!=-2 && off[0]!=firstOffset)) std::abort();
}

static void exact_cases(Fn fn) {
    expect(fn,{u'A',u'B'},1,0,U_BUFFER_OVERFLOW_ERROR,1,1,0,0);
    expect(fn,{u'A',char16_t(0x100)},2,0,U_INVALID_CHAR_FOUND,2,1,0x100,0);
    expect(fn,{char16_t(0xdc00)},2,0,U_ILLEGAL_CHAR_FOUND,1,0,0xdc00,999);
    expect(fn,{char16_t(0xd800)},2,0,U_ZERO_ERROR,1,0,0xd800,999);
    expect(fn,{char16_t(0xdc00)},1,0xd800,U_INVALID_CHAR_FOUND,1,0,0x10000,999);
    expect(fn,{u'A'},1,0xd800,U_ILLEGAL_CHAR_FOUND,0,0,0xd800,999);
    expect(fn,{u'A'},0,0xd800,U_BUFFER_OVERFLOW_ERROR,0,0,0xd800,999);
}

static void check(const std::array<char16_t, 256> &input, int n, int cap,
                  bool latin, UChar32 pending, bool withOffsets,
                  UErrorCode initial, int targetByte=0, int offsetByte=-1) {
    struct Result {
        alignas(4) std::array<unsigned char, 2048> mem{};
        UChar32 state;
        UErrorCode error;
        int src, dst, off;
    } r[2];
    for(int k=0;k<2;++k) {
        auto &v=r[k];
        v.mem.fill(0xa5);
        // Input at byte 256; target/offset placement optionally exercises
        // byte-wise source/target overlap and aligned offset aliasing.
        std::memcpy(v.mem.data()+256,input.data(),512);
        int tb=targetByte ? targetByte : 800;
        int ob=offsetByte>=0 ? offsetByte : 1200;
        if(offsetByte<0) std::memset(v.mem.data()+ob,0x5a,768);
        UConverter cnv{latin ? &_Latin1Data : &_ASCIIData,pending};
        auto *src=reinterpret_cast<const char16_t *>(v.mem.data()+256);
        auto *off=withOffsets ? reinterpret_cast<int32_t *>(v.mem.data()+ob) : nullptr;
        UConverterFromUnicodeArgs args{&cnv,src,src+n,
            reinterpret_cast<char *>(v.mem.data()+tb),
            reinterpret_cast<char *>(v.mem.data()+tb+cap),off};
        UErrorCode err=initial;
#ifdef STANDALONE
        _Latin1FromUnicodeWithOffsets_rvv(&args,&err);
#else
        (k ? _Latin1FromUnicodeWithOffsets_rvv : _Latin1FromUnicodeWithOffsets)(&args,&err);
#endif
        v.state=cnv.fromUChar32;
        v.error=err;
        v.src=(int)(args.source-src);
        v.dst=(int)(args.target-reinterpret_cast<char *>(v.mem.data()+tb));
        v.off=withOffsets ? (int)(args.offsets-off) : -1;
#ifdef STANDALONE
        // Independently check ordinary one-to-one success (including offsets).
        if(pending==0 && initial==U_ZERO_ERROR && n<=cap &&
           targetByte==0 && offsetByte<0 &&
           std::all_of(input.begin(),input.begin()+n,
                       [latin](char16_t x) { return x <= (latin ? 255 : 127); })) {
            if(err!=U_ZERO_ERROR || v.src!=n || v.dst!=n ||
               (withOffsets && v.off!=n)) std::abort();
            for(int i=0;i<n;++i) {
                if(v.mem[tb+i]!=static_cast<uint8_t>(input[i]) ||
                   (withOffsets && off[i]!=i)) std::abort();
            }
        }
#endif
    }
#ifndef STANDALONE
    if(r[0].mem!=r[1].mem || r[0].state!=r[1].state ||
       r[0].error!=r[1].error || r[0].src!=r[1].src ||
       r[0].dst!=r[1].dst || r[0].off!=r[1].off) {
        std::fprintf(stderr,"mismatch n=%d cap=%d latin=%d pending=%x offsets=%d "
                     "target=%d off=%d src=%d/%d dst=%d/%d error=%d/%d\n",
                     n,cap,latin,pending,withOffsets,targetByte,offsetByte,
                     r[0].src,r[1].src,r[0].dst,r[1].dst,
                     r[0].error,r[1].error);
        std::abort();
    }
#endif
}

int main() {
    exact_cases(_Latin1FromUnicodeWithOffsets_rvv);
#ifndef STANDALONE
    exact_cases(_Latin1FromUnicodeWithOffsets);
#endif
    std::mt19937 rng(0x12778);
    std::array<char16_t,256> input{};
    for(bool latin : {false,true}) for(bool off : {false,true}) {
        for(int n : {0,1,7,15,16,17,31,32,33,63,64,127,191}) {
            for(int cap : {0,1,7,15,16,17,31,32,33,64,200}) {
                for(int pos : {-1,0,1,7,15,16,17,31,32,63,100,190}) {
                    for(auto &c:input) c=static_cast<char16_t>(rng()%(latin?256:128));
                    if(pos>=0 && pos<n) input[pos]=u'\u0100';
                    check(input,n,cap,latin,0,off,U_ZERO_ERROR);
                }
            }
        }
        for(char16_t bad : {char16_t(0x100),char16_t(0xd800),
                             char16_t(0xdc00),char16_t(0xdbff)}) {
            for(int pos : {0,1,15,16,31,32,48}) {
                input.fill(u'x'); input[pos]=bad;
                for(char16_t next : {char16_t('a'),char16_t(0xdc00),char16_t(0xdfff)}) {
                    input[pos+1]=next;
                    for(int cap : {0,1,15,16,17,32,49,64})
                        check(input,65,cap,latin,0,off,U_ZERO_ERROR);
                }
            }
        }
        for(UChar32 pending : {0xd800,0xdbff,0x100,0xdc00}) {
            for(char16_t first : {char16_t(0xdc00),char16_t('x'),char16_t(0xd800)}) {
                input.fill(u'x'); input[0]=first;
                for(int cap : {0,1,16,32,64})
                    check(input,40,cap,latin,pending,off,U_ZERO_ERROR);
            }
        }
        input.fill(u'x');
        for(int n : {0,1,15,16,17,31,32,33,64})
            check(input,n,64,latin,0,off,U_INVALID_CHAR_FOUND);
#ifndef STANDALONE
        // Aliasing is legal for the extracted raw-pointer function when all
        // accessed storage is live; compare even the rolled-back bytes.
        for(int tb : {256,257,258,280,300}) {
            input.fill(u'x'); input[21]=0x100;
            check(input,64,48,latin,0,off,U_ZERO_ERROR,tb);
        }
        if(off) {
            input.fill(u'x');
            check(input,64,48,latin,0,true,U_ZERO_ERROR,800,256);
            check(input,64,48,latin,0,true,U_ZERO_ERROR,800,800);
        }
#endif
        for(int it=0;it<2000;++it) {
            int n=static_cast<int>(rng()%192), cap=static_cast<int>(rng()%193);
            for(auto &c:input) c=static_cast<char16_t>(rng()%0xe000);
            if(it%3==0) for(int j=0;j<n;++j)
                input[j]=static_cast<char16_t>(rng()%(latin?256:128));
            check(input,n,cap,latin,0,off,U_ZERO_ERROR);
        }
    }
    std::puts("ok");
}
