#include <stdio.h>
#include <string.h>

#include <message.h>

#define llong long long

#include "rps.h"

int id, nodes, M;
int paritition;

char A[1<<22];
llong B[1<<22];

void run(llong all) {
   while (all > 1 ) {
      for (int i = 0; i < all; i+=2) {
         if (A[i] == A [i + 1]) {
            A[i/2] = A[i];
            B[i/2] = B[i];
         } else if ((A[i] == 'R' && A[i + 1] == 'S') || (A[i] == 'S' && A[i + 1] == 'P') || (A[i] == 'P' && A[i + 1] == 'R')) {
            A[i/2] = A[i];
            B[i/2] = B[i];
         } else {
            A[i/2] = A[i + 1];
            B[i/2] = B[i + 1];
         }
      }
      all = all / 2;
   }
}

void master() {
   for (int i = 0 ; i < paritition; i++) {
      Receive(i);
      A[i] = GetChar(i);
      B[i] = GetLL(i);
   }
   run(paritition);
   printf("%lld\n", B[0]);
}

void worker() {
   if (id >= paritition) return;
   int n = GetN();
   llong total = 1L;
   for (int  i = 0 ; i < n; i++) total = total *2;
   llong avg = total / paritition;
   for (llong i = 0 ; i < avg; i++) {
      A[i] = GetFavoriteMove(avg * id + i);
      B[i] = avg * id + i;
   }
   run(avg);
   // printf("%c %lld\n", A[0], B[0]);
   PutChar(M, A[0]);
   PutLL(M, B[0]);
   Send(M);
}

int main() {
    id = MyNodeId();
    nodes = NumberOfNodes() - 1;
    paritition = 1;
    int n = GetN();
    llong total = 1L;
    for (int  i = 0 ; i < n; i++) total = total *2;
    while ((2 * paritition <= nodes) && (2 * paritition < total)) {
        paritition = paritition * 2;
    }

    M = nodes;
    if (id == nodes) {
      master();
    } else {
      worker();
    } 
}
