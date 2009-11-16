#define verbose 0 \

#define foreach(i,c) for(typeof((c) .begin() ) i= (c) .begin() ;i!=(c) .end() ;++i)  \

#define in(e,c) ((c) .find(e) !=(c) .end() )  \

/*2:*/
#line 37 "./ascirc.w"

#include <map> 
#include <set> 
#include <queue> 
#include <stdio.h> 
#include <assert.h> 

using namespace std;

/*7:*/
#line 113 "./ascirc.w"

struct Node{
char op;
Node*left;
Node*right;

Node(char op,Node*left,Node*right)
:op(op),left(left),right(right){}
bool operator<(const Node&o)const{
if(op!=o.op)return op<o.op;
if(left!=o.left)return left<o.left;
return right<o.right;
}
};

map<Node,Node*> hashcons;

Node*mk_node(char op,Node*left,Node*right){
Node*r= new Node(op,left,right);
map<Node,Node*> ::iterator it= hashcons.find(*r);
if(it!=hashcons.end()){
delete r;
return it->second;
}
return hashcons[*r]= r;
}

/*:7*/
#line 46 "./ascirc.w"


int main(){
/*3:*/
#line 59 "./ascirc.w"

int tests;
scanf("%d",&tests);
while(tests--){
foreach(n,hashcons)delete n->second;
hashcons.clear();
Node*out[256];
for(char c= 'a';c<='z';++c)out[c]= mk_node(c,NULL,NULL);
int assignments;
scanf("%d",&assignments);
while(assignments--)/*4:*/
#line 76 "./ascirc.w"
{
char asg[5];
scanf("%s",asg);
out[asg[3]]= mk_node(asg[0],out[asg[1]],out[asg[2]]);
}

/*:4*/
#line 69 "./ascirc.w"
;
/*5:*/
#line 86 "./ascirc.w"

set<Node*> internal_nodes;
queue<Node*> q;
for(char c= 'a';c<='z';++c)if(out[c]->left!=NULL){
if(!in(out[c],internal_nodes))
q.push(out[c]);
internal_nodes.insert(out[c]);
}
while(!q.empty()){
Node*n= q.front();q.pop();
Node*nn= n->left;
/*6:*/
#line 104 "./ascirc.w"

if(nn->left!=NULL&&!in(nn,internal_nodes)){
q.push(nn);
internal_nodes.insert(nn);
}

/*:6*/
#line 97 "./ascirc.w"
;
nn= n->right;
/*6:*/
#line 104 "./ascirc.w"

if(nn->left!=NULL&&!in(nn,internal_nodes)){
q.push(nn);
internal_nodes.insert(nn);
}

/*:6*/
#line 99 "./ascirc.w"
;
}
printf("%d\n",internal_nodes.size());


/*:5*/
#line 70 "./ascirc.w"
;
}

/*:3*/
#line 49 "./ascirc.w"
;
}

/*:2*/
