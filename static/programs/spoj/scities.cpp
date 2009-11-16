#include <algorithm>
#include <iostream>
#include <cstdlib>
using namespace std;
#define o   ++mems
#define oo  mems += 2
#define ooo mems += 3

#define W(r, c) (ooo, w[r][c] - row_dec[r] + col_inc[c])

#define MEMSET(var, val) \
  mems += sizeof(var) / sizeof(int), memset(var, val, sizeof(var))

const int MAX = 100;

const int INF = 0x7fffffff; // Almost infinity

int mems;  // Memory references

int M;           // Number of rows.
int N;           // Number of columns.
int w[MAX][MAX]; // The weights (MxN adjacency matrix)
int r;           // Row of interest.
int c;           // Column of interest.

int t;                  // Number of unchosen rows.
int unchosen_rows[MAX];
int col_mate[MAX];
int row_mate[MAX];
int parent_row[MAX];

int row_dec[MAX];
int col_inc[MAX];
int slack[MAX];

int unmatched_rows; // The number of rows yet to be matched.

bool verbose = false;


int main() {
  int tests; cin >> tests;
  for (int i = 0; i < tests; ++i) {
    cin >> M >> N;
    bool transpose = false;
    if (M > N) {
      swap(M, N);
      transpose = true;
    }
    for (r = 0; r < M; ++r) for (c = 0; c < N; ++c)
      o, w[r][c] = MAX;
    while (true) {
      int weight;
      cin >> r >> c >> weight;
      if (!r) break;
      if (transpose) swap(r, c);
      o, w[r-1][c-1] = MAX - weight;
    }

    MEMSET(col_inc, 0);
    MEMSET(row_dec, 0);
    MEMSET(col_mate, -1);
    MEMSET(row_mate, -1);
    unmatched_rows = M;

    while (unmatched_rows) {
      int q;        // Number of unchosen rows that were explored.
      MEMSET(parent_row, -1);
      for (c = 0; c < N; ++c) o, slack[c] = INF;
      t = 0;
      for (r = 0; r < M; ++r) if (o, col_mate[r] == -1)
        o, unchosen_rows[t++] = r;

      for (q = 0; q < t; ++q) {
        o, r = unchosen_rows[q];
        for (c = 0; c < N; ++c) if (o, W(r, c) < slack[c]) {
          o, slack[c] = W(r, c);
          if (o, slack[c] == 0) {
            o, parent_row[c] = r;
            if (o, row_mate[c] == -1) goto done_bfs;
            oo, unchosen_rows[t++] = row_mate[c];
          }
        }
      }
       done_bfs:

      if (q == t) { // No improving path found.
        int del = INF;
        for (c = 0; c < N; ++c) if (o, parent_row[c] == -1)
          if (o, slack[c] < del) o, del = slack[c];
        for (c = 0; c < N; ++c) if (o, parent_row[c] != -1)
          o, col_inc[c] += del;
        for (q = 0; q < t; ++q) oo, row_dec[unchosen_rows[q]] += del;

      } else {
        o, r = parent_row[c];
        while (o, col_mate[r] != -1) {
          int nc;
          o, nc = col_mate[r];
          o, row_mate[c] = r;
          o, col_mate[r] = c;

          c = nc;
          o, r = parent_row[c];
        }
        o, row_mate[c] = r;
        o, col_mate[r] = c;

        --unmatched_rows;

      }
      if (verbose) 
        fprintf(stderr, 
                "After %d mems there are %d matched rows\n",
                mems, M - unmatched_rows
                );

    }

    int result  = 0;
    for (r = 0; r < M; ++r) 
      oo, result += MAX - w[r][col_mate[r]];
    cout << result << endl;

    if (verbose)
      fprintf(stderr, "I used %d mems to solve test %d.\n", mems, i+1);
    mems = 0;

  }
}

