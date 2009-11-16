#define infty 0x7fffffff \

/*2:*/
#line 11 "./minqueue.w"

#include <assert.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 

/*3:*/
#line 40 "./minqueue.w"

struct Node{
int count;
int min_value;
Node*prev;
Node*next;
};

Node*oldest;
Node*newest;

/*:3*/
#line 17 "./minqueue.w"


void push(int x)/*6:*/
#line 81 "./minqueue.w"
{
int count= 1;
Node*p,*q;
p= newest;
while(p&&p->min_value>=x){
count+= p->count;
q= p;
p= q->prev;
free(q);
if(p)p->next= NULL;
}
newest= p;
p= (Node*)malloc(sizeof(Node));
p->count= count;
p->min_value= x;
if(newest)newest->next= p;
p->prev= newest;
p->next= NULL;
if(!newest)oldest= p;
newest= p;
}/*:6*/
#line 19 "./minqueue.w"

void pop()/*5:*/
#line 66 "./minqueue.w"
{
assert(oldest);
if(oldest->count==1){
Node*todelete= oldest;
oldest= oldest->next;
if(oldest)oldest->prev= NULL;
if(!oldest)newest= NULL;
free(todelete);
}else
--oldest->count;
}

/*:5*/
#line 20 "./minqueue.w"

int min()/*4:*/
#line 59 "./minqueue.w"
{
if(oldest)return oldest->min_value;
return infty;
}

/*:4*/
#line 21 "./minqueue.w"


int main(){
int v;
char buf[1<<8];
while(fgets(buf,1<<8,stdin)){
if(sscanf(buf,"PUSH %d",&v)==1)
push(v);
else if(!memcmp(buf,"POP",3))
pop();
else if(!memcmp(buf,"MIN",3))
printf("%d\n",min());
else
printf("I don't understand: %s\n",buf);
}
}

/*:2*/
