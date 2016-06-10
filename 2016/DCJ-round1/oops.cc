#include <stdio.h>
#include <string.h>

#include <message.h>

#define llong long long

#include "oops.h"

int id, nodes, M;
llong MAXV;

void master() {
  llong max = -MAXV;
  llong min = MAXV;
  for (int i = 0 ; i < nodes; i ++) {
      Receive(i);
      llong v = GetLL(i);
      if (v < min) min = v;
      v = GetLL(i);
      if (v > max) max = v;
   }
   printf("%lld\n", max - min);
}

void worker() {
  llong n = GetN();
  llong avg = (n + nodes - 1) / nodes;
  llong max = -MAXV;
  llong min = MAXV;
  for (llong i = id * avg; i < n && i < (id + 1 ) * avg; i ++) {
      llong v = GetNumber(i);
      if (v > max) max = v;
      if (v < min) min = v;
  }
  PutLL(M, min);
  PutLL(M, max);
  Send(M);
}

int main() {
    MAXV = 1L * 1000000 * 1000000 * 1000000 + 100000L;
    id = MyNodeId();
    nodes = NumberOfNodes() - 1;
    M = nodes;
    if (id == nodes) {
      master();
    } else {
      worker();
    } 
}
