#include <stdio.h>
#include <string.h>
#include <math.h>
#include <algorithm>
using namespace std;

#include <message.h>
#define MOD 1000000007L

#define llong long long

#include "crates.h"

int id, nodes, M;

llong A[10240000];
llong B[128];

void master() {
   llong total = 0;
   for (int i = 0; i < nodes; i++) {
     Receive(i);
     B[i] = GetLL(i);
     total += B[i];
   }
   llong sum = 0;
   for (int i = 0 ; i < nodes; i++) {
      PutLL(i, total);
      PutLL(i, sum);
      Send(i);
      sum+= B[i];
   }
   llong ret = 0;
   for (int i = 0; i < nodes; i++) {
     Receive(i);
     ret = (ret + GetLL(i))% MOD;
   }
   printf("%lld\n", ret);
}

void worker() {
   llong n = GetNumStacks();
   llong total = 0;
   llong avg = (n + nodes - 1) / nodes;
   llong start = avg * id;
   for (llong i = start; i < avg * (id + 1) && i < n; i++) {
      A[i - start] = GetStackHeight(i + 1);
      total += A[i - start];
   }
   PutLL(M, total);
   Send(M);
   
   Receive(M);
   total = GetLL(M);
   llong sum1 = GetLL(M);
   llong ret = 0;
   for (llong i = start; i < avg * (id + 1) && i < n; i++) {
       llong sum2 = total / n  * (i + 1) + min(i + 1, total % n);
       sum1 += A[i - start];
       if (sum1 > sum2 ) {
         ret = (ret + (sum1 - sum2 + MOD) % MOD) % MOD;
       } else {
         ret = (ret + (sum2 - sum1 + MOD) % MOD) % MOD;
       }
   }
   PutLL(M, ret);
   Send(M);
   // printf("%d %lld\n", id, ret);
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
