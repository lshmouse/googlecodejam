#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include <message.h>
#include <set>
#include <map>

using namespace std;

#define llong long long

#include "shhhh.h"

int id, nodes, M;
int seeds[4096];
map<int, int> m;

void master() {
  int n = GetN();
  int* to = new int[nodes];
  int* steps = new int[nodes];
  for (int i = 0; i < nodes; i++) {
    Receive(i);
    to[i] = GetLL(i);
    steps[i] = GetLL(i);
    // printf("%d %d %d\n", seeds[i], seeds[to[i]], steps[i]);
  }

  int start = 0;
  int step = 0;
  while (start != -1) {
     step += steps[start];
     start = to[start];
  }
  if (2*step == n) {
    printf("WHATEVER %d\n", step);
  } else if (step < n - step) {
    printf("RIGHT %d\n", step);
  } else {
    printf("LEFT %d\n", n - step);
  }
  delete[] to;
  delete[] steps;
}

void worker() {
  int start =  seeds[id];
  int step = 0;
  while (start != 1) {
    start = GetRightNeighbour(start);
    step++;
    if (m.find(start) != m.end()) {
       int to = m[start];
       PutLL(M, to);
       PutLL(M, step);
       Send(M);
       return;
    }
  }
  // end 1
  PutLL(M, -1);
  PutLL(M, step);
  Send(M);
}

void init() {
    srand(0);
    int n = GetN();
    seeds[0] = 0;
    m[0] = 0;
    for (int i = 1; i < nodes; i ++) {
       seeds[i] = rand() %(n - 1) + 1;
       m[seeds[i]] = i;
    }
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
