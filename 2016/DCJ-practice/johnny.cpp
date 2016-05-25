#include <stdio.h>
#include <algorithm>
#include <message.h>

using namespace std;

#define llong long long

#include "johnny.h"

int id, nodes, M;
int A[20480];

void master() {
  int n = NumberOfCards();
  for (int i = 0; i < n; i ++) {
    Receive(i % nodes);
    A[i] = GetInt(i % nodes);
  }
  sort(A, A + n);
  if (A[0] == A[n-1]) {
    printf("IMPOSSIBLE\n");
  } else {
    int sum = A[0];
    for (int i = 1; i < n; i++){
      if (A[i] >= i && A[i-1] < i && sum == (i * (i-1)/2)) {
        printf("%d\n", n - i);
        break;
      }
      sum += A[i];
    }
  }
}

void worker() {
  int n = NumberOfCards();
  for (int i = 0;i < n; i++) if ((i % nodes) == id) {
    int out = 0;
    for (int j = 0; j < n; j ++) if (i != j) {
      if (IsBetter(i, j)) out++;
    }
    PutInt(M, out);
    Send(M);
  }
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
