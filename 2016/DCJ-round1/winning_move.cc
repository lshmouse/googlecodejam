#include <stdio.h>
#include <string.h>
#include <algorithm>

using namespace std;

#include <message.h>

#define llong long long

#include "winning_move.h"

int id, nodes, M;

llong A[1000000];

void master() {
   llong ret = 0;
   for (int i = 0; i < nodes; i++) {
       Receive(i);
       llong tmp = GetLL(i);
       if (tmp == 0) continue;
       if (ret == 0 || tmp < ret) ret = tmp;
   }
   printf("%lld\n", ret);
}

void worker() {
   llong n = GetNumPlayers();
   llong avg = (n + nodes -1) / nodes;
   int sz = 0;
   for (llong i = id * avg; i < n && i < (id +1) * avg; i++) {
       A[sz++] = GetSubmission(i);
   }
   sort(A, A + sz);
   int count = 1;
   for (int i = 1; i < sz; i++) {
      if (A[i] == A[i-1]) {
        count++;
      } else {
        PutLL(A[i-1] % nodes, A[i-1]);
        if (count > 1) PutLL(A[i-1] % nodes, A[i-1]);
        count = 1;
      }
   }
   if (sz > 0) {
     PutLL(A[sz-1] % nodes, A[sz-1]);
     if (count > 1) PutLL(A[sz-1] % nodes, A[sz-1]);
   }
   for (int i = 0 ; i < nodes; i++) {
     PutLL(i, 0);
     Send(i);
   }
   sz = 0;
   for (int i = 0 ; i < nodes; i++) {
     Receive(i);
     llong tmp;
     while ((tmp = GetLL(i)) > 0) {
       A[sz++]= tmp;
     }
   }
   sort(A, A + sz);
   llong ret = 0;
   for (int i = 0; i < sz; i++) {
      if ((i == 0 || (A[i] > A[i-1])) && ((i == sz - 1) || (A[i] < A[i+1]))) {
        ret = A[i];
        break;
      }
   }
   PutLL(M, ret);
   Send(M);
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
