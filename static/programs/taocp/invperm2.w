% vim: spell

\def\o#1{\overline{#1}}

@* Intro. Computes the inverse of a permutation in place using
algorithm J from TAoCP. This is also a solution to exercise
1.3.3-13 that asks for a proof of correctness.

@ Here is how the algorithm finds the inverse permutation of
$024135$ (here $\o x =-x-1$):

$$\o{024135} \to^5 \o{02413}5 \to^4 \o{024}4\o15
\to^3 \o03\o44\o25 \to^2 \o03\o2425
\to^1 \o031425 \to^0 031425$$

@ The array |x| initially contains the permutation. To express the
invariant we introduce the notation

$$x_i=\hbox{the initial value of |x[i]|}$$
$$P(a,b)=\hbox{if |x[a]<0| then |x[a]==|$\o b$ else $P(|x[a]|,b)$}$$

\noindent we maintain the invariants

$$\hbox{|x[i]>=0|}\quad\hbox{implies}\quad x_|x[i]|=i,\qquad
  \hbox{for $0\le i<n$}$$
$$P(i,x_i),\qquad\hbox{for $0\le i\le m$}$$

\noindent The size of |x| is denoted by~|n|. The variable~$m$
starts from~$n-1$ and goes down to~$0$. In each main iteration a
negative value in the array is made positive. Therefore at the
end all values will be positive and the first invariant says
that we'll have the inverse permutation in~|x|. Initially the
invariants are established by assigning |x[i]=-x[i]-1| for all~$i$.
(The first invariant is trivially satisfied because false
implies anything and the second invariant is satisfied because
the `if' branch of $P$ is taken for all~$i$.)

@ At each step we use one negative value~|x[j]| to figure
out where to place the nonnegative~|m|. When we write |m| a
negative value would be overwritten if we wouldn't save it in
the place~|j|. (The value~|x[j]| is not going to be needed again
anyway --- it served its purpose.)

@ If we know that $P(m,x_m)$ then we can find $x_m$ by inspecting
the array |x| starting with |j=m| and continuing to set |j=x[j]|
until |x[j]<0|. (See the definition of~$P$.) Then we can set
$x[x_m]=m$, making some progress in constructing the inverse
permutation. However, this might destroy the invariant 
$P(x_m,x_{x_m})$ (and it is possible that $x_m<m$). Since
$m$ is nonnegative $P(x_m,x_{x_m})$ is $P(m,x_{x_m})$. But
|x[m]| was just used to read $x_m$ so it is now available to
hold $\o{x_{x_m}}$, which re-establishes the invariant.

@ The above explanation really makes sense only after you've done
a few examples by hand. The proof really needs to be improved.

@ The program itself is pretty simple.

@c
#include <stdio.h>
#include <stdlib.h>

int main() {
  int n; /* size of the permutation */
  int m; /* the |m| appearing in the proof */
  int i, j; /* indices */
  int *x; 
  @<Read the permutation@>;
  @<Compute the inverse@>;
  @<Print the inverse@>;
}

@ We trust the user with correct input. Otherwise the universe
might collapse.

@<Read...@>=
scanf("%d",&n);
if (n < 1) return 1;
x = (int*)malloc(n*sizeof(int));
for (i = 0; i < n; ++i) scanf("%d",&x[i]);

@ @<Compute...@>=
for (i = 0; i < n; ++i) x[i] = -x[i]-1;
for (m = n - 1; m >= 0; --m) {
  for (i = m, j = x[m]; j >= 0; i = j, j = x[j]);
  x[i] = x[-j-1], x[-j-1] = m;
}

@ @<Print...@>=
for (i = 0; i < n; ++i) printf("%d\n", x[i]);

