#include <stdio.h>
#include <math.h>
#define N 3
int main(void){
    double a[N][N]={{2,1,-1},{-3,-1,2},{-2,1,2}};
    double b[N]={8,-11,-3}, l[N][N]={{0}}, u[N][N]={{0}}, y[N], x[N];
    int p[N]={0,1,2};
    for(int i=0;i<N;i++)l[i][i]=1;
    for(int k=0;k<N;k++){
        int q=k;for(int i=k+1;i<N;i++)if(fabs(a[i][k])>fabs(a[q][k]))q=i;
        if(fabs(a[q][k])<1e-12){puts("Singular matrix.");return 1;}
        if(q!=k){for(int j=0;j<N;j++){double t=a[k][j];a[k][j]=a[q][j];a[q][j]=t;}
            for(int j=0;j<k;j++){double t=l[k][j];l[k][j]=l[q][j];l[q][j]=t;}
            int t=p[k];p[k]=p[q];p[q]=t;}
        for(int j=k;j<N;j++)u[k][j]=a[k][j];
        for(int i=k+1;i<N;i++){l[i][k]=a[i][k]/u[k][k];for(int j=k;j<N;j++)a[i][j]-=l[i][k]*u[k][j];}
    }
    for(int i=0;i<N;i++){double s=b[p[i]];for(int j=0;j<i;j++)s-=l[i][j]*y[j];y[i]=s;}
    for(int i=N-1;i>=0;i--){double s=y[i];for(int j=i+1;j<N;j++)s-=u[i][j]*x[j];x[i]=s/u[i][i];}
    puts("Solution from PA=LU:");for(int i=0;i<N;i++)printf("x%d = %.2f\n",i+1,x[i]);return 0;
}
