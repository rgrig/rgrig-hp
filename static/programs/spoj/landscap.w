\datethis

\input epsf

@* Overview. This is a solution to the 
\pdfURL{LANDSCAP}{https://www.spoj.pl/problems/LANDSCAP/}
SPOJ problem. It is also my first \.{CWEB} program.

@ A `landscape' is an array of $N$ `elevations', 
where $1\le N\le 1000$. We must level the landscape so that 
$\le L$ `peaks' remain, where $1\le L\le 25$. A peak is a constant
region of the landscape that is delimited on both sides 
by a smaller elevation or by the edge of the landscape.

For the specification of the input/output format, an example,
and other details please see the problem statement at SPOJ.

@ In the first phase the program transforms the landscape into
a rooted and weighed tree. For example
  $$\epsfbox{landscap.0}$$
\noindent
comes from the following partition of the sample input:
  $$\epsfbox{landscap.1}$$
Each node corresponds to a rectangular region of the landscape.
Its weight is the area of the rectangular region.
Its children are the rectangular regions that directly sit on it.
We choose the height of the rectangular regions greedily maximal.

If we denote by $A(n)$ the maximum number of nodes that can
be obtained from a landscape of length $n$ we have
$$A(n)=\max_{n_1+\cdots+n_d\le n-d}  A(n_1)+\cdots+A(n_d)+1
\qquad\hbox{for some $d\ge0$}$$
We can now prove by induction that $A(n)\le n$.

For the running time we have the following recurrence.
  $$B(n)=B(n_1)+\cdots+B(n_d)+O(n)$$
We can now prove that $B(n)=O(n^2)$. Here is a sketch of
a proof that assumes $d>0$.

$$\eqalign{
B(n) &= B(n_1)+\cdots+B(n_d)+n\cr
     &\le Cn^2_1+\cdots+Cn^2_d+Cn\cr
     &\le C(n^2-2nd-d^2-d(d-1)+n)\cr
     &= C(n^2+(n+d)(1-2d))\cr
     &\le Cn^2\cr
}$$

(Is there a tighter bound?)

@ The second phase finds the answer by processing the tree.
An equivalent problem is the following: What is the minimum
weight we need to cut from the tree so that only $\le L$ leafs 
remain? In fact, we will solve the (roughly) opposite
problem: What is the maximum weight of a sub-tree that has
exactly $l$ leafs? This problem is hard so we generalize
to make it easier: What is the maximum weight of a sub-tree
of the tree rooted in $x$ that has exactly $l$ leafs, provided
it may use only the first $c$ children of $x$? Let's denote
this quantity by $m(x,l,c)$. In the following formul\ae\ the
weight of $x$ is denoted by $w$, the $c$-th child of $x$ is
denoted by $y$, and the number of children of $y$ is denoted
by~$d$.

$$\eqalign{
m(x,0,c)&=0\cr
m(x,l,c)&=\max\left(w + m(y,l,d),
  \max_{0<k\le l} \big(m(x,k,c-1)+m(y,l-k,d)\big)\right)
\qquad\hbox{for $c>0$ and $l>0$}\cr
m(x,l,0)&=\cases{
  0, & for $l=0$\cr
  w, & for $l=1$\cr
  -\infty, & otherwise}\cr}$$

In general, for a node $x$ we will denote its weight by $w_x$ and
the number of its children by $d_x$.

@ How many $m$-values are there? The node $x$ can take $N$ values;
the number of allowed leafs can take $L+1$ values, from $0$ to $L$; the 
children limit $c$ is nonnegative and $\le d_x$.

$$\sum_{x=0}^{N-1}\sum_{l=0}^L\sum_{c=0}^{d_x} 1 =
(L+1) \sum_{x=0}^{N-1} (d_x+1) = (L+1)(2N-1)$$

(The number of edges in a tree with $N$ nodes is $N-1$.)
So there are $(L+1)(2N-1)$ values to compute. Each value is computed
in $\le L$ steps, therefore all are computed in $O(NL^2)$ steps.

@ The overall structure of the code reflects the two phase 
process---first construct a tree out of the landscape and then
process the tree.

@d max_N 1024 // conservative strict upper limit for |N|
@d max_L 32 // conservative strict upper limit for |L|

@c
@<Includes@>@;
using namespace std;
@h
int land[max_N]; // the landscape
int N; // landscape length
int L; // maximum number of peaks
int* m[max_N][max_L]; // the values $m(x,l,c)$ presented above
@<Tree data@>@;
@<List operations@>;
@<Tree construction@>@;

int main() {
  int tests; @+cin >> tests;
  while (tests--) {
    @<Local variables@>;
    @<Read input and construct tree@>;
    @<Allocate memory for |m|@>;
    @<Compute |m|@>;
    @<Print solution@>;
    @<Cleanup@>;
  }
}

@* Tree construction. The tree can be represented by an array
of lists with children. (A matrix representation would require
$\Omega(N^2)$ space while this approach uses $\Theta(N)$ space.)

@<Tree data@>=
struct Node { 
  Node(Node* n, int c) @+ : n(n), c(c) {}
  int c; // child
  Node* n; // next
};
Node* children[max_N]; 
  // |children[x]| is a linked list with the children of |x|
int d[max_N]; 
  // number of children (|d[x]| is the length of the list |children[x]|)
int w[max_N]; // node weights
int nodes_cnt; // number of nodes

@ The construction is done recursively by a function that
processes the interval $[\,a.\,.\>b)$ of |land|.

@d infty 0x7fffffff // almost infinity

@<Tree construction@>=
void make_tree(int a, int b) {
  int i, j;
  int n = nodes_cnt++; // current node
  int m = infty;

  for (i = a; i < b; ++i) m = min(m, land[i]);
  for (i = a; i < b; ++i) land[i] -= m;

  w[n] = m * (b-a);
  j = a;
  while (true) {
    for (i = j; i < b && !land[i]; ++i);
    if (i == b) return;
    for (j = i; j < b && land[j]; ++j);
    children[n] = add(children[n], nodes_cnt); @+ ++d[n];
    make_tree(i, j);
  }
}

@ The tree construction is done immediately after reading the input.

@<Read input...@>=
cin >> N >> L;
for (int i = 0; i < N; ++i) cin >> land[i];
make_tree(0, N);


@* Tree processing. The values |m| can be computed by dynamic
programming. Note that below we use |-1| instead of $-\infty$;
that should be small enough.  We also use that all children of
$x$ are~$>x$.

@<Local variables@>=
int x; // current node
Node* y; // current child
int l; // limit on number of leafs for the first |c| children
int k; // limit on number of leafs for the first |c-1| children
int c; // current number of considered children

@ @<Compute |m|@>=
for (x = 0; x < nodes_cnt; ++x) {
  m[x][0][0] = 0; @+ m[x][1][0] = w[x];
  for (l = 2; l <= L; ++l) m[x][l][0] = -1;
}

for (x = nodes_cnt - 1; x >= 0; --x) {
  y = children[x];
  for (c = 1; c <= d[x]; ++c) {
    m[x][0][c] = 0;
    for (l = 1; l <= L; ++l) {
      m[x][l][c] = w[x] + m[y->c][l][d[y->c]];
      for (k = 1; k <= l; ++k)
        m[x][l][c] = max(m[x][l][c], 
          m[x][k][c-1] + m[y->c][l-k][d[y->c]]);
    }
    y = y->n;
  }
}

@ The minimum weight that needs to be cut form the tree to obtain
a tree with $\le L$ leafs is obtained by subtracting from the
total weight the maximal weight of a tree with $0$, $1$, $\ldots$, or
$L$ leafs.

@<Print...@>=
int total_weight = 0;
int max_weight = -1;
for (x = 0; x < nodes_cnt; ++x) total_weight += w[x];
for (l = 0; l <= L; ++l) max_weight = max(max_weight, m[0][l][d[0]]);
cout << (total_weight - max_weight) << endl;


@* Memory management and list operations. We need to allocate 
memory for |m| after we construct the tree (so that we know
how big the individual arrays should be).

@<Allocate...@>=
for (l = 0; l <= L; ++l) {
  for (x = 0; x < nodes_cnt; ++x)
    m[x][l] = (int*)malloc((d[x] + 1) * sizeof(int));
}

@ At the end we must free this memory plus the memory used for
representing the adjacency lists. Also, we reset all the other
data structures that are expected to be reset when starting to
solve a test case.

@<Cleanup@>=
for (x = 0; x < nodes_cnt; ++x) {
  del(children[x]); @+ children[x] = NULL;
  for (l = 0; l <= L; ++l)
    free(m[x][l]);
}
nodes_cnt = 0;
memset(d, 0, sizeof(d));
memset(children,0,sizeof(children));

@ The only two list operations are inserting an element and deleting
the whole lost. The brevity of these two functions is an argument
that list operations are easy.

@<List operations@>=
Node* add(Node* p, int c) {
  return new Node(p, c);
}

void del(Node* p) {
  Node* n;
  while (p) {
    n = p->n;
    delete p;
    p = n;
  }
}


@* Loose ends. 

@<Includes@>=
#include <iostream>
#include <vector>
#include <algorithm> // for |min|
#include <cstdlib>
