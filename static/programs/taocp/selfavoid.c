/*2:*/
#line 40 "./selfavoid.w"

#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 

typedef int restype;


int m,n;
int stop;
restype cache[20][1<<20];



restype solve(int pos,int seen){
if(pos==stop)return 1;
if(seen&(1<<pos))return 0;
if(cache[pos][seen]!=-1)return cache[pos][seen];
seen|= 1<<pos;
/*3:*/
#line 81 "./selfavoid.w"

restype&r= cache[pos][seen]= 0;
if(pos+n<m*n)r+= solve(pos+n,seen);
if(pos-n>=0)r+= solve(pos-n,seen);
if(pos%n)r+= solve(pos-1,seen);
if((pos+1)%n)r+= solve(pos+1,seen);
return r;

/*:3*/
#line 60 "./selfavoid.w"
;
}

int main(){
scanf("%d %d\n",&m,&n);
if(m*n<=0||m*n> 20){
fprintf(stderr,
"The size of the grid (%dx%d) should be >0 and <=20.\n",m,n);
return 1;
}
for(int x= 0;x<(m+1)/2;++x)for(int y= 0;y<(n+1)/2;++y)
for(stop= 0;stop<m*n;++stop){
memset(cache,-1,sizeof(cache));
printf("(%d,%d)->(%d,%d)=%d\n",
x,y,stop/n,stop%n,solve(n*x+y,0));
}
}

/*:2*/
