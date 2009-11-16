#define max 37
/*2:*/
#line 14 "./strings1.w"

#include <stdio.h> 
#include <string.h> 

typedef long long i64;

i64 cache[4][max+1][max+1];

i64 solve(int previous,int zero_left,int one_left){
i64&r= cache[previous][zero_left][one_left];
if(r!=-1)return r;
if(!zero_left&&!one_left)return r= 1;
r= 0;
if(zero_left&&(previous&1))r+= solve(1,zero_left-1,one_left);
if(one_left&&(previous&2))r+= solve(2,zero_left,one_left-1);
if(zero_left&&one_left)r+= solve(3,zero_left-1,one_left-1);
return r;
}

int main(){
int n;
scanf("%d",&n);
if(n> max){
fprintf(stderr,"Sorry, but %d is too big for me.\n",n);
return 1;
}
memset(cache,-1,sizeof(cache));
for(int i= 1;i<=n;++i)
printf("n %d cnt %lld\n",i,solve(3,i,i));
}

/*:2*/
