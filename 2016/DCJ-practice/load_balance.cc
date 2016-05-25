#include <stdio.h>
#include <algorithm>
#include <string.h>
#include <set>
#include <vector>
#include <map>

using namespace std;

#include "load_balance.h"
#include <message.h>

#define llong long long
#define MAXM 1000
int id, nodes, M;
llong A[64], sum;
llong half;
set<llong> a, b, c;
llong B[1<<15];
int MOD, n;
vector<llong> L1[MAXM], R1[MAXM], L2[MAXM], R2[MAXM];

void master() {
  if (sum % 2 == 1) {
    printf("IMPOSSIBLE\n");
    return;
  }
  bool flag = false;
  for (int i = 0; i < nodes; i++) {
    Receive(i);
    if (GetInt(i) == 1) flag = true;
  }
  if (flag) {
    printf("POSSIBLE\n");
  } else {
    printf("IMPOSSIBLE\n");
  }
}

void dfs(llong* A, int s, int t, vector<llong>* all) {
  int sz = (t - s);
  if (sz < 0) sz = 0;
  B[0] = 0;
  c.clear();
  c.insert(0);
  for (int i = 0; i < sz ; i++) {
    B[1<<i] = A[s + i];
  }
  for (int i = 1; i < (1<<sz); i++) {
    int x = i & (i-1);
    B[i] = B[x]+ B[i - x];
    if (B[i] <= half) {
      c.insert(B[i]);
    }
  }
  for (set<llong>::const_iterator itr = c.begin(); itr != c.end(); itr++) {
    all[(*itr) % MOD].push_back(*itr);
  }
}

void search(vector<llong>* L, vector<llong>* R, int key, set<llong>* a) {
  llong counter = 0;
  for (int i = 0; i < MOD; i++) {
     int j = (key - i + MOD) % MOD;
     for (vector<llong>::const_iterator itr1 = L[i].begin(); itr1 != L[i].end(); itr1++) {
       for (vector<llong>::const_iterator itr2 = R[j].begin(); itr2 != R[j].end(); itr2++) {
         llong sum = (*itr1) + (*itr2);
         counter++;
         if (sum <= half) {
           a->insert(sum);
         }
       }
     }
  }
  //printf("%lld\n", counter);
}

bool once(int key) {
  int k = n / 2;
  a.clear();
  search(L1, R1, key, &a);
  b.clear();
  key = (half % MOD - key + MOD) % MOD;
  search(L2, R2, key, &b);

  for (set<llong>::const_iterator itr = a.begin(); itr != a.end(); itr++) {
    llong v = half - (*itr);
    if (b.find(v) != b.end()) {
      return true;
    }
  }
  return false;
}

void worker() {
  if (sum % 2 == 1) return;
  half = sum / 2;
  dfs(A, 0, n/4, L1);
  dfs(A, n/4, n/2, R1);
  dfs(A, n/2, 3 * n / 4, L2);
  dfs(A, 3 * n / 4, n, R2);
  
  int flag = 0;
  for(int i = 0; i < MOD; i++) {
    if ((i % nodes) == id && once(i)) {
      flag = 1;
      break;
    }
  }
  PutInt(M, flag);
  Send(M);
}

void init() {
  n = GetN();
  sum = 0;
  for (int i = 0; i < n; i++) {
    A[i] = GetWeight(i);
    sum += A[i];
  }
  MOD = 419;
}

int main() {
  id = MyNodeId();
  nodes = NumberOfNodes() - 1;
  M = nodes;
  init();
  if (id == nodes) {
    master();
  } else {
    worker();
  } 
}
// bad case
// n = 52
// GetWeight(i) = (2LL<<i)
