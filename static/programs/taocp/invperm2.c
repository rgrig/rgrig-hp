/*7:*/
#line 59 "./invperm2.w"

#include <stdio.h> 
#include <stdlib.h> 

int main(){
int n;
int m;
int i,j;
int*x;
/*8:*/
#line 76 "./invperm2.w"

scanf("%d",&n);
if(n<1)return 1;
x= (int*)malloc(n*sizeof(int));
for(i= 0;i<n;++i)scanf("%d",&x[i]);

/*:8*/
#line 68 "./invperm2.w"
;
/*9:*/
#line 82 "./invperm2.w"

for(i= 0;i<n;++i)x[i]= -x[i]-1;
for(m= n-1;m>=0;--m){
for(i= m,j= x[m];j>=0;i= j,j= x[j]);
x[i]= x[-j-1],x[-j-1]= m;
}

/*:9*/
#line 69 "./invperm2.w"
;
/*10:*/
#line 89 "./invperm2.w"

for(i= 0;i<n;++i)printf("%d\n",x[i]);
/*:10*/
#line 70 "./invperm2.w"
;
}

/*:7*/
