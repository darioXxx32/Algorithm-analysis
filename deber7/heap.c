#include <stdio.h>
#define N 10
void heapify(int a[],int n,int i){
    while(1){int largest=i,l=2*i+1,r=2*i+2;
        if(l<n&&a[l]>a[largest])largest=l;
        if(r<n&&a[r]>a[largest])largest=r;
        if(largest==i)return;
        int t=a[i];a[i]=a[largest];a[largest]=t;i=largest;
    }
}
int main(void){int a[N]={4,10,3,5,1,8,2,7,6,9};
    for(int i=N/2-1;i>=0;i--)heapify(a,N,i);
    printf("Max-heap array: ");for(int i=0;i<N;i++)printf("%d%s",a[i],i==N-1?"\n":" ");return 0;
}
