#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define o    (verbose ? ++mems : 0)
#define oo   (verbose ? mems += 2 : 0)
#define ooo  (verbose ? mems += 3 : 0)
#define oooo (verbose ? mems += 4 : 0)
#define MEMSET(c, v) \
  (verbose ? (mems += sizeof(c) / sizeof(int) + 1) : 0), \
  memset(c, v, sizeof(c))

const int MAXLCM = 663;

const int CARDS = 52;
const int COUNT = 13;
const int PLAYERS = 10;

const bool verbose = false;

int mems;   // Memory references.

struct Node {
  Node(int v) : v(v) {}
  int v;
  Node *p, *n; // Previous and next.
};
Node* top[PLAYERS];
int last[PLAYERS];
int counter[PLAYERS];

int players; // The total number of players.
int p; // The current player of interest.
int c; // The current card of interest.

Node* insert_after(Node* nn, Node* n) {
  if (!n) {
    oo, nn->p = nn->n = nn;
  } else {
    o,    nn->p = n;
    oo,   nn->n = n->n;
    oooo, nn->p->n = nn->n->p = nn;
  }
  return nn;
}

Node* insert_before(Node* nn, Node* n) {
  if (n) o, n = n->p;
  return insert_after(nn, n);
}

Node* extract(Node* n) {
  if (!n) return NULL;
  Node* np;
  o, np = n->p;
  if (np == n) return NULL;
  ooo, (np->n = n->n)->p = np;
  return np;
}

Node* delete_list(Node* n) {
  if (n) {
    delete_list(extract(n));
    delete n;
  }
  return NULL;
}


int main() {
  int tests; scanf("%d ", &tests);
  for (int t = 0; t < tests; ++t) {
    scanf("%d ", &players);
    for (c = 0; c < CARDS; ++c) {
      int v; scanf("%d ", &v);
      ooo, top[0] = insert_after(new Node(v-1), top[0]);
    }
    ooo,  top[0] = top[0]->n;

    MEMSET(counter, 0);
    int discard_time, time;
    discard_time = time = 0;
    int to_discard = players * CARDS;
    Node* handed_out[PLAYERS]; 
    while (to_discard && time <= discard_time + MAXLCM) {
      ++time;
      MEMSET(handed_out, 0);
      for (p = 0; p < players; ++p) if (o, top[p]) {
        if (ooo, top[p]->v == counter[p]) {
          if (verbose)
            fprintf(stderr, "Player %d discards %d.\n", p+1, counter[p]+1);
          oooo, last[p] = (handed_out[p] = top[p])->v;
          oo, top[p] = extract(top[p]);
          --to_discard;
          discard_time = time;
        }
        if (o, top[p]) ooo, top[p] = top[p]->n;
        o, ++counter[p] %= COUNT;
      }
      for (p = 1; p < players; ++p) if (o, handed_out[p-1]) {
        if (verbose)
          fprintf(stderr, "Take card handed by player %d.\n", p);
        ooo, top[p] = insert_before(handed_out[p-1], top[p]);
        ooo, top[p] = top[p]->n;
      }
      oo, delete handed_out[players-1];
    }

    if (to_discard)
      printf("Case %d: unwinnable\n", t+1);
    else {
      printf("Case %d:", t+1);
      for (p = 0; p < players; ++p)
        o, printf(" %d", last[p] + 1);
      printf("\n");
    }

    if (to_discard) {
      for (p = 0; p < players; ++p)
        oo, top[p] = delete_list(top[p]);
    }

    if (verbose) {
      fprintf(stderr, "I used %d mems to solve case %d.\n", mems, t+1);
      mems = 0; 
    }

  }
}

