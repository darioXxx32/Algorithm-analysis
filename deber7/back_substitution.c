#include <stdio.h>
#define N 3
int main(void) {
    const double u[N][N]={{2,1,-1},{0,.5,.5},{0,0,-1}};
    const double b[N]={8,1,1};
    double x[N];
    for(int i=N-1;i>=0;i--){double s=b[i];for(int j=i+1;j<N;j++)s-=u[i][j]*x[j];x[i]=s/u[i][i];}
    printf("Solution: x = %.2f, y = %.2f, z = %.2f\n",x[0],x[1],x[2]);
    return 0;
}
