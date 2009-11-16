/*6:*/
#line 95 "./landscap.w"

/*16:*/
#line 260 "./landscap.w"

#include <iostream> 
#include <vector> 
#include <algorithm>  
#include <cstdlib> /*:16*/
#line 96 "./landscap.w"

using namespace std;
#define max_N 1024
#define max_L 32 \

#define infty 0x7fffffff \


#line 98 "./landscap.w"

int land[max_N];
int N;
int L;
int*m[max_N][max_L];
/*7:*/
#line 123 "./landscap.w"

struct Node{
Node(Node*n,int c):n(n),c(c){}
int c;
Node*n;
};
Node*children[max_N];

int d[max_N];

int w[max_N];
int nodes_cnt;

/*:7*/
#line 103 "./landscap.w"

/*15:*/
#line 243 "./landscap.w"

Node*add(Node*p,int c){
return new Node(p,c);
}

void del(Node*p){
Node*n;
while(p){
n= p->n;
delete p;
p= n;
}
}


/*:15*/
#line 104 "./landscap.w"
;
/*8:*/
#line 141 "./landscap.w"

void make_tree(int a,int b){
int i,j;
int n= nodes_cnt++;
int m= infty;

for(i= a;i<b;++i)m= min(m,land[i]);
for(i= a;i<b;++i)land[i]-= m;

w[n]= m*(b-a);
j= a;
while(true){
for(i= j;i<b&&!land[i];++i);
if(i==b)return;
for(j= i;j<b&&land[j];++j);
children[n]= add(children[n],nodes_cnt);++d[n];
make_tree(i,j);
}
}

/*:8*/
#line 105 "./landscap.w"


int main(){
int tests;cin>>tests;
while(tests--){
/*10:*/
#line 174 "./landscap.w"

int x;
Node*y;
int l;
int k;
int c;

/*:10*/
#line 110 "./landscap.w"
;
/*9:*/
#line 163 "./landscap.w"

cin>>N>>L;
for(int i= 0;i<N;++i)cin>>land[i];
make_tree(0,N);


/*:9*/
#line 111 "./landscap.w"
;
/*13:*/
#line 218 "./landscap.w"

for(l= 0;l<=L;++l){
for(x= 0;x<nodes_cnt;++x)
m[x][l]= (int*)malloc((d[x]+1)*sizeof(int));
}

/*:13*/
#line 112 "./landscap.w"
;
/*11:*/
#line 181 "./landscap.w"

for(x= 0;x<nodes_cnt;++x){
m[x][0][0]= 0;m[x][1][0]= w[x];
for(l= 2;l<=L;++l)m[x][l][0]= -1;
}

for(x= nodes_cnt-1;x>=0;--x){
y= children[x];
for(c= 1;c<=d[x];++c){
m[x][0][c]= 0;
for(l= 1;l<=L;++l){
m[x][l][c]= w[x]+m[y->c][l][d[y->c]];
for(k= 1;k<=l;++k)
m[x][l][c]= max(m[x][l][c],
m[x][k][c-1]+m[y->c][l-k][d[y->c]]);
}
y= y->n;
}
}

/*:11*/
#line 113 "./landscap.w"
;
/*12:*/
#line 206 "./landscap.w"

int total_weight= 0;
int max_weight= -1;
for(x= 0;x<nodes_cnt;++x)total_weight+= w[x];
for(l= 0;l<=L;++l)max_weight= max(max_weight,m[0][l][d[0]]);
cout<<(total_weight-max_weight)<<endl;


/*:12*/
#line 114 "./landscap.w"
;
/*14:*/
#line 229 "./landscap.w"

for(x= 0;x<nodes_cnt;++x){
del(children[x]);children[x]= NULL;
for(l= 0;l<=L;++l)
free(m[x][l]);
}
nodes_cnt= 0;
memset(d,0,sizeof(d));
memset(children,0,sizeof(children));

/*:14*/
#line 115 "./landscap.w"
;
}
}

/*:6*/
