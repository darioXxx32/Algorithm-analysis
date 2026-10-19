#include <stdio.h>
#include <math.h>
#define N 3
int main(void) {
    double a[N][N+1] = {{2,1,-1,8},{-3,-1,2,-11},{-2,1,2,-3}};
    for (int k=0;k<N-1;k++) {
        int p=k;
        for (int i=k+1;i<N;i++) if (fabs(a[i][k])>fabs(a[p][k])) p=i;
        if (fabs(a[p][k])<1e-12) { puts("No unique solution."); return 1; }
        if (p!=k) for (int j=k;j<=N;j++){double t=a[k][j];a[k][j]=a[p][j];a[p][j]=t;}
        for (int i=k+1;i<N;i++) { double m=a[i][k]/a[k][k]; a[i][k]=0;
            for (int j=k+1;j<=N;j++) a[i][j]-=m*a[k][j]; }
    }
    if (fabs(a[N-1][N-1])<1e-12) { puts("No unique solution."); return 1; }
    puts("Upper triangular system:");
    for(int i=0;i<N;i++) printf("%.2f %.2f %.2f | %.2f\n",a[i][0],a[i][1],a[i][2],a[i][N]);
    return 0;
}
