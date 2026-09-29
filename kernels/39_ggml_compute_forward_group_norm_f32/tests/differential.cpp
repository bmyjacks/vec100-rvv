#include "kernel.h"
#include <cstdio>
#include <cstring>
void ggml_compute_forward_group_norm_f32_bench_rvv(const float*,float*,int64_t,int64_t,int,int,float);
static unsigned s=0x196c4au;
static unsigned rnd(){s^=s<<13;s^=s>>17;s^=s<<5;return s;}
int main() {
    for(int t=0;t<170;++t) {
        int64_t width=1+rnd()%37,rows=1+rnd()%4;
        int channels=1+rnd()%9,groups=1+rnd()%channels;
        float a[2048],b[2048],c[2048];
        for(float &x:a)x=(int(rnd()%2001)-1000)/37.f;
        for(float &x:b)x=-999;
        memcpy(c,b,sizeof(b));
        float eps=t%3?1e-5f:0.f;
        ggml_compute_forward_group_norm_f32_bench(a,b,width,rows,channels,groups,eps);
        ggml_compute_forward_group_norm_f32_bench_rvv(a,c,width,rows,channels,groups,eps);
        for (int i=0;i<2048;++i) if (memcmp(&b[i],&c[i],sizeof(float))) {
            fprintf(stderr,"group norm mismatch trial=%d index=%d scalar=%a rvv=%a\n",t,i,b[i],c[i]);
            return 1;
        }
    }
    puts("group norm OK");
}
