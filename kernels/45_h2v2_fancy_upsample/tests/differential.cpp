#include "kernel.h"
#include <cstdio>
#include <cstring>
void h2v2_fancy_upsample_rvv(j_decompress_ptr,jpeg_component_info*,_JSAMPARRAY,_JSAMPARRAY*);
static unsigned s=0x8df2abu;
static unsigned rnd(){s^=s<<13;s^=s>>17;s^=s<<5;return s;}
int main() {
    for(int t=0;t<200;++t) {
        jpeg_decompress_struct info = {}; info.max_v_samp_factor=2*(1+rnd()%4)-(t%5==0);
        jpeg_component_info comp = {};comp.downsampled_width=2+rnd()%75;
        JSAMPLE in[7][80], a[8][160],b[8][160];
        JSAMPROW rows[7],ra[8],rb[8];
        for(int i=0;i<7;++i){rows[i]=in[i];for(auto &x:in[i])x=rnd();}
        for(int i=0;i<8;++i){ra[i]=a[i];rb[i]=b[i];memset(a[i],0xa5,160);memset(b[i],0xa5,160);}
        _JSAMPARRAY input=rows+1,oa=ra,ob=rb;
        h2v2_fancy_upsample(&info,&comp,input,&oa);
        h2v2_fancy_upsample_rvv(&info,&comp,input,&ob);
        for (int row=0;row<8;++row) for (int col=0;col<160;++col)
            if (a[row][col] != b[row][col]) {
                fprintf(stderr,"upsample mismatch trial=%d row=%d col=%d scalar=%u rvv=%u\n",
                        t,row,col,unsigned(a[row][col]),unsigned(b[row][col]));
                return 1;
            }
    }
    puts("upsample OK");
}
