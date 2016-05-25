#include <stdio.h>
#include <algorithm>

using namespace std;

#include "almost_sorted.h"
#include <message.h>

#define llong long long

#define MOD (1L<<20)

int id, nodes, M;

void master() {
   llong ret = 0;
   for (int i = 0; i < nodes; i++) {
      Receive(i);
      ret = (ret + GetLL(i)) % MOD;
   }
   printf("%lld\n", ret);
}

void worker() {
  int n = NumberOfFiles();
  int dist = MaxDistance() + 1;
  int avg = (n + nodes -1 ) / nodes;
  int start = avg * id;
  int end = min(n, avg * (id  + 1));
  int ss = max(0, start - dist);
  int ee = min(n, end + dist);
  llong* B = new llong[ee-ss];
  int count = 0;
  for (int i = ss; i < ee; i ++) {
     B[count++] = Identifier(i);
  }
  sort(B, B + count);
  llong ret = 0;
  for (int i = start - ss; i < end - ss; i ++) {
    ret =  (ret + ((ss + i)% MOD) * (B[i] % MOD) % MOD) % MOD;
  }
  PutLL(M, ret);
  Send(M);
  delete[] B;
}

int main() {
    id = MyNodeId();
    nodes = NumberOfNodes() - 1;
    M = nodes;
    if (id == nodes) {
      master();
    } else {
      worker();
    } 
}
