#include "kernel.h"
#include <cstdint>
#include <cstdio>
#include <cstring>
extern "C" void skl_gemm_f8e4m3rc_f8e4m3rc_f32rc_ref_rvv(
 size_t,size_t,size_t,float,const uint8_t*,size_t,size_t,const uint8_t*,size_t,size_t,float,float*,size_t,size_t);
static uint32_t s=0x34ad80u;
static uint32_t rnd() { s^=s<<13;s^=s>>17;s^=s<<5;return s; }
int main() {
    for(int t=0;t<180;++t) {
        size_t m=1+rnd()%5,n=rnd()%30,k=rnd()%8;
        uint8_t a[1024],b[1024]; float c[1024],d[1024];
        for(int i=0;i<1024;++i) {
            a[i]=rnd();b[i]=rnd();c[i]=int(rnd()%31)-15;d[i]=c[i];
        }
        float alpha=(t%3?0.5f:0.f),beta=(t%4?-0.25f:1.f);
        #define ARGS(dst) m,n,k,alpha,a,59,2,b,61,2,beta,dst,62,2
        skl_gemm_f8e4m3rc_f8e4m3rc_f32rc_ref(ARGS(c));
        skl_gemm_f8e4m3rc_f8e4m3rc_f32rc_ref_rvv(ARGS(d));
        for (size_t i=0;i<sizeof(c)/sizeof(c[0]);++i)
            if (memcmp(&c[i],&d[i],sizeof(c[i]))) {
                fprintf(stderr,"FP8 GEMM mismatch trial=%d index=%zu scalar=%a rvv=%a\n",t,i,c[i],d[i]);
                return 1;
            }
        #undef ARGS
    }
    puts("FP8 GEMM OK");
}
