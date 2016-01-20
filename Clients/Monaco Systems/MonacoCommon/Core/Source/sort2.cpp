// sort2.c
// heap sort from numerical recipies in C
#include "sort2.h"


void sort2(int n,double *ra,long *rb)
{
        int l,j,ir,i;
       	double rra;
       	long   rrb;

        l=(n >> 1)+1;
        ir=n;
        for (;;) {
                if (l > 1) {
                        rra=ra[--l];
                        rrb=rb[l];
                } else {
                        rra=ra[ir];
                        rrb=rb[ir];
                        ra[ir]=ra[1];
                        rb[ir]=rb[1];
                        if (--ir == 1) {
                                ra[1]=rra;
                                rb[1]=rrb;
                                return;
                        }
                }
                i=l;
                j=l << 1;
                while (j <= ir) {
                        if (j < ir && ra[j] < ra[j+1]) ++j;
                        if (rra < ra[j]) {
                                ra[i]=ra[j];
                                rb[i]=rb[j];
                                j += (i=j);
                        }
                        else j=ir+1;
                }
                ra[i]=rra;
                rb[i]=rrb;
        }
}

void sort(int n,double *ra)
{
        int l,j,ir,i;
       	double rra;

        l=(n >> 1)+1;
        ir=n;
        for (;;) {
                if (l > 1) {
                        rra=ra[--l];
                } else {
                        rra=ra[ir];
                        ra[ir]=ra[1];
                        if (--ir == 1) {
                                ra[1]=rra;
                                return;
                        }
                }
                i=l;
                j=l << 1;
                while (j <= ir) {
                        if (j < ir && ra[j] < ra[j+1]) ++j;
                        if (rra < ra[j]) {
                                ra[i]=ra[j];
                                j += (i=j);
                        }
                        else j=ir+1;
                }
                ra[i]=rra;
        }
}