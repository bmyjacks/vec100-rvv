#include "kernel.h"
#include <cstdint>
#include <cstdio>
#include <cstring>

extern "C" void skl_gemm_f32rcprc_f32rcprc_f32rcprc_ref_rvv(
 size_t,size_t,size_t,size_t,size_t,size_t,float,const float*,size_t,size_t,size_t,size_t,
 const float*,size_t,size_t,size_t,size_t,float,float*,size_t,size_t,size_t,size_t);
static uint32_t s = 0x912ad3u;
static uint32_t rnd() { s ^= s << 13; s ^= s >> 17; s ^= s << 5; return s; }
int main() {
    for (int t = 0; t < 220; ++t) {
        size_t m0=1+rnd()%4,n0=rnd()%30,k0=rnd()%6,m1=1+rnd()%3,n1=1+rnd()%3,k1=rnd()%4;
        size_t rsa0=27,csa0=2,rsa1=300,csa1=46,rsb0=31,csb0=2,rsb1=300,csb1=47;
        size_t rsc0=29,csc0=(t%2 ? 2 : 1),rsc1=350,csc1=43;
        float a[2048],b[2048],c[2048],d[2048];
        for (int i=0;i<2048;++i) {
            a[i]=(int(rnd()%2001)-1000)/731.f; b[i]=(int(rnd()%2001)-1000)/523.f;
            c[i]=(int(rnd()%2001)-1000)/419.f; d[i]=c[i];
        }
        float alpha = (t%3 ? -0.75f : 0.f), beta = (t%4 ? 1.25f : 0.f);
        #define ARGS(dst) m0,n0,k0,m1,n1,k1,alpha,a,rsa0,csa0,rsa1,csa1,b,rsb0,csb0,rsb1,csb1,beta,dst,rsc0,csc0,rsc1,csc1
        skl_gemm_f32rcprc_f32rcprc_f32rcprc_ref(ARGS(c));
        skl_gemm_f32rcprc_f32rcprc_f32rcprc_ref_rvv(ARGS(d));
        for (int i=0;i<2048;++i) if (memcmp(&c[i],&d[i],sizeof(float))) {
            fprintf(stderr,"packed FP32 GEMM mismatch trial=%d index=%d scalar=%a rvv=%a\n",t,i,c[i],d[i]);
            return 1;
        }
        #undef ARGS
    }
    puts("packed FP32 GEMM OK");
}
