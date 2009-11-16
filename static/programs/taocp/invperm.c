/*1:*/
#line 14 "./invperm.w"

#include <stdio.h> 

int p[100];
int m[100];
int n;

int main(){
int x,y,py;
scanf("%d ",&n);
for(x= 0;x<n;++x)scanf("%d ",&p[x]);
x= 0;
while(1){
while(x<n&&m[x])++x;
if(x==n)break;
y= p[x],py= p[y];
while(!m[y]){
p[y]= x,m[y]= 1;
x= y,y= py,py= p[y];
}
}
for(x= 0;x<n;++x)printf("%d ",p[x]);
printf("\n");
}

/*:1*/
