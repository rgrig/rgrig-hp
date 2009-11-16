/*1:*/
#line 12 "./perm.w"

#include <stdio.h> 

int iperm[256];
int perm[256];

int main(){
/*2:*/
#line 31 "./perm.w"

int start;
int prev;
int current;
int t;
for(current= 0;current<256;++current)iperm[current]= current;
/*3:*/
#line 48 "./perm.w"

do current= getchar();
while(!((current>='a'&&current<='z')
||current==')'||current=='('||current==EOF));
if(current==EOF)goto out;

/*:3*/
#line 37 "./perm.w"
;
while(1){
while(current=='('){
/*3:*/
#line 48 "./perm.w"

do current= getchar();
while(!((current>='a'&&current<='z')
||current==')'||current=='('||current==EOF));
if(current==EOF)goto out;

/*:3*/
#line 40 "./perm.w"
;
start= current;
}
if(current==')')current= start;
t= iperm[current];iperm[current]= prev;prev= t;
/*3:*/
#line 48 "./perm.w"

do current= getchar();
while(!((current>='a'&&current<='z')
||current==')'||current=='('||current==EOF));
if(current==EOF)goto out;

/*:3*/
#line 45 "./perm.w"
;
}

/*:2*/
#line 19 "./perm.w"
;
out:
for(int i= 0;i<256;++i)perm[iperm[i]]= i;
/*4:*/
#line 56 "./perm.w"

while(1){
for(start= 0;start<256&&perm[start]==start;++start);
if(start==256)break;
printf("(%c",start);
for(current= perm[start];current!=start;current= perm[current]){
perm[iperm[current]]= iperm[current];
printf("%c",current);
}
perm[iperm[start]]= iperm[start];
printf(")");
}
printf("\n");


/*:4*/
#line 22 "./perm.w"
;
}

/*:1*/
