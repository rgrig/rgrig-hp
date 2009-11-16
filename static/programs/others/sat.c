/*3:*/
#line 21 "./sat.w"

/*9:*/
#line 110 "./sat.w"

#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 

/*:9*/
#line 22 "./sat.w"

#define lit_head(l) ((l) > 0?&positive[l]:&negative[-(l) ])  \
 \

#define verbose 0 \


#line 23 "./sat.w"

/*4:*/
#line 44 "./sat.w"

struct lit_node{
struct lit_node*pc;
struct lit_node*nc;
struct lit_node*p;
struct lit_node*n;
struct clause_node*clause;
int literal;
};

struct clause_node{
struct clause_node*p;
struct clause_node*n;
struct lit_node*clause;
int size;
};

/*:4*/
#line 24 "./sat.w"

/*5:*/
#line 64 "./sat.w"

struct clause_node*formula;

struct lit_node*positive;


struct lit_node*negative;



/*:5*//*6:*/
#line 80 "./sat.w"

int var_cnt;
int clause_cnt;
int max_clause_size;
struct clause_node*all_clauses;

/*:6*/
#line 25 "./sat.w"

/*19:*/
#line 283 "./sat.w"

void printformula(int depth){
for(int d= 0;d<depth;++d)printf(" ");
for(int sz= 0;sz<=max_clause_size;++sz){
for(struct clause_node*cn= formula[sz].n;cn!=&formula[sz];cn= cn->n){
for(struct lit_node*ln= cn->clause->nc;ln!=cn->clause;ln= ln->nc){
printf(" %d",ln->literal);
}
printf("|");
}
}
printf("\n");
}

/*:19*/
#line 26 "./sat.w"

/*13:*/
#line 166 "./sat.w"

int sat(int depth){
int l;
/*17:*/
#line 267 "./sat.w"

int sz;
if(formula[0].n!=&formula[0])return 0;
for(sz= 1;sz<=max_clause_size&&formula[sz].n==&formula[sz];++sz);
if(sz> max_clause_size)return 1;

/*:17*/
#line 169 "./sat.w"
;
/*18:*/
#line 276 "./sat.w"

l= formula[sz].n->clause->nc->literal;

/*:18*/
#line 170 "./sat.w"
;
/*14:*/
#line 189 "./sat.w"
{
struct lit_node*ln,*lln;
struct clause_node*cn,*ccn;
struct clause_node rc;
int rsat;
rc.p= rc.n= &rc,rc.clause= NULL;
if(verbose){
for(int d= 0;d<depth;++d)printf(" ");
printf("set %d\n",l);
}
/*15:*/
#line 219 "./sat.w"

for(ln= lit_head(-l)->n;ln!=lit_head(-l);ln= ln->n){
ln->pc->nc= ln->nc,ln->nc->pc= ln->pc;
ln->pc= ln->nc= ln;
cn= ln->clause;
cn->p->n= cn->n,cn->n->p= cn->p;
--cn->size;
cn->p= &formula[cn->size],cn->n= formula[cn->size].n;
cn->p->n= cn->n->p= cn;
}
for(ln= lit_head(l)->n;ln!=lit_head(l);ln= ln->n){
cn= ln->clause;
for(lln= cn->clause->nc;lln!=cn->clause;lln= lln->nc)
if(lln->literal!=ln->literal){
lln->p->n= lln->n,lln->n->p= lln->p;
lln->n= lln->p= lln;
}
cn->p->n= cn->n,cn->n->p= cn->p;
cn->p= &rc,cn->n= rc.n;
cn->p->n= cn->n->p= cn;
}

/*:15*/
#line 199 "./sat.w"
;
if(verbose)printformula(depth);
rsat= sat(depth+1);
if(verbose){
for(int d= 0;d<depth;++d)printf(" ");
printf("unset %d\n",l);
}
/*16:*/
#line 243 "./sat.w"

for(cn= rc.n;cn!=&rc;cn= ccn){
for(ln= cn->clause->nc;ln!=cn->clause;ln= ln->nc)
if(ln->literal!=l){
ln->p= lit_head(ln->literal),ln->n= lit_head(ln->literal)->n;
ln->p->n= ln->n->p= ln;
}
ccn= cn->n;
cn->p= &formula[cn->size],cn->n= formula[cn->size].n;
cn->p->n= cn->n->p= cn;
}
for(ln= lit_head(-l)->n;ln!=lit_head(-l);ln= ln->n){
cn= ln->clause;
ln->pc= cn->clause,ln->nc= cn->clause->nc;
ln->pc->nc= ln->nc->pc= ln;
cn->p->n= cn->n,cn->n->p= cn->p;
++cn->size;
cn->p= &formula[cn->size],cn->n= formula[cn->size].n;
cn->p->n= cn->n->p= cn;
}

/*:16*/
#line 206 "./sat.w"
;
if(verbose)printformula(depth);
if(rsat)return 1;
}

/*:14*/
#line 171 "./sat.w"
;
l= -l;
/*14:*/
#line 189 "./sat.w"
{
struct lit_node*ln,*lln;
struct clause_node*cn,*ccn;
struct clause_node rc;
int rsat;
rc.p= rc.n= &rc,rc.clause= NULL;
if(verbose){
for(int d= 0;d<depth;++d)printf(" ");
printf("set %d\n",l);
}
/*15:*/
#line 219 "./sat.w"

for(ln= lit_head(-l)->n;ln!=lit_head(-l);ln= ln->n){
ln->pc->nc= ln->nc,ln->nc->pc= ln->pc;
ln->pc= ln->nc= ln;
cn= ln->clause;
cn->p->n= cn->n,cn->n->p= cn->p;
--cn->size;
cn->p= &formula[cn->size],cn->n= formula[cn->size].n;
cn->p->n= cn->n->p= cn;
}
for(ln= lit_head(l)->n;ln!=lit_head(l);ln= ln->n){
cn= ln->clause;
for(lln= cn->clause->nc;lln!=cn->clause;lln= lln->nc)
if(lln->literal!=ln->literal){
lln->p->n= lln->n,lln->n->p= lln->p;
lln->n= lln->p= lln;
}
cn->p->n= cn->n,cn->n->p= cn->p;
cn->p= &rc,cn->n= rc.n;
cn->p->n= cn->n->p= cn;
}

/*:15*/
#line 199 "./sat.w"
;
if(verbose)printformula(depth);
rsat= sat(depth+1);
if(verbose){
for(int d= 0;d<depth;++d)printf(" ");
printf("unset %d\n",l);
}
/*16:*/
#line 243 "./sat.w"

for(cn= rc.n;cn!=&rc;cn= ccn){
for(ln= cn->clause->nc;ln!=cn->clause;ln= ln->nc)
if(ln->literal!=l){
ln->p= lit_head(ln->literal),ln->n= lit_head(ln->literal)->n;
ln->p->n= ln->n->p= ln;
}
ccn= cn->n;
cn->p= &formula[cn->size],cn->n= formula[cn->size].n;
cn->p->n= cn->n->p= cn;
}
for(ln= lit_head(-l)->n;ln!=lit_head(-l);ln= ln->n){
cn= ln->clause;
ln->pc= cn->clause,ln->nc= cn->clause->nc;
ln->pc->nc= ln->nc->pc= ln;
cn->p->n= cn->n,cn->n->p= cn->p;
++cn->size;
cn->p= &formula[cn->size],cn->n= formula[cn->size].n;
cn->p->n= cn->n->p= cn;
}

/*:16*/
#line 206 "./sat.w"
;
if(verbose)printformula(depth);
if(rsat)return 1;
}

/*:14*/
#line 173 "./sat.w"
;
return 0;
}

/*:13*/
#line 27 "./sat.w"


int main(){
/*7:*/
#line 86 "./sat.w"

/*8:*/
#line 93 "./sat.w"

char linebuf[1<<10];
max_clause_size= 0;
while(fgets(linebuf,1<<10,stdin)&&strncmp("p cnf",linebuf,5));
if(sscanf(linebuf,"p cnf %d %d",&var_cnt,&clause_cnt)!=2){
fprintf(stderr,"Input must contain\np cnf <varcnt> <clausecnt>\n");
return 1;
}
all_clauses= malloc(clause_cnt*sizeof(struct clause_node));
positive= malloc((var_cnt+1)*sizeof(struct lit_node));
negative= malloc((var_cnt+1)*sizeof(struct lit_node));
for(int i= 1;i<=var_cnt;++i){
positive[i].n= positive[i].p= &positive[i];
negative[i].n= negative[i].p= &negative[i];
positive[i].literal= negative[i].literal= 0;
}

/*:8*/
#line 87 "./sat.w"
;
/*10:*/
#line 115 "./sat.w"

for(int i= 0;i<clause_cnt;++i){
struct clause_node*clause= &all_clauses[i];

clause->size= 0;
clause->clause= malloc(sizeof(struct lit_node));
clause->clause->pc= clause->clause->nc= clause->clause;
clause->clause->literal= 0;
while(1)/*11:*/
#line 133 "./sat.w"
{
int l;
struct lit_node*literal;
if(scanf("%d",&l)!=1||!l)break;
literal= malloc(sizeof(struct lit_node));
literal->literal= l;
literal->clause= clause;
literal->p= lit_head(l),literal->n= lit_head(l)->n;
literal->p->n= literal->n->p= literal;
literal->nc= clause->clause->nc,literal->pc= clause->clause;
literal->pc->nc= literal->nc->pc= literal;
++clause->size;
}

/*:11*/
#line 123 "./sat.w"
;
if(max_clause_size<clause->size)
max_clause_size= clause->size;
}

/*:10*/
#line 88 "./sat.w"
;
/*12:*/
#line 147 "./sat.w"

formula= malloc((max_clause_size+1)*sizeof(struct clause_node));
for(int i= 0;i<=max_clause_size;++i){
formula[i].p= formula[i].n= &formula[i];
formula[i].clause= NULL;
}
for(struct clause_node*c= all_clauses;c!=all_clauses+clause_cnt;++c){
c->n= formula[c->size].n,c->p= &formula[c->size];
c->p->n= c->n->p= c;
}

/*:12*/
#line 89 "./sat.w"
;

/*:7*/
#line 30 "./sat.w"
;
printf(sat(0)?"SAT\n":"UNSAT\n");
/*21:*/
#line 298 "./sat.w"


/*:21*/
#line 32 "./sat.w"
;
return 0;
}

/*:3*/
