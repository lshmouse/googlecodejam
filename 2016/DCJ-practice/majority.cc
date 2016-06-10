#include <stdio.h>

#include "majority.h"
#include <message.h>

#define llong long long

int id, nodes, M;
llong n;

void master() {
    llong ret = -1, count = 0;
    for (int i = 0; i < nodes; i ++) {
      Receive(i);
      llong v = GetLL(i);
      llong c = GetLL(i);
      if (count == 0) {
        ret = v;
        count = c;
      } else if (ret == v) {
        count+= c;
      } else if (ret != v) {
        if (count >=c) {
           count = count -c;
        } else {
           count = c - count;
           ret = v;
        }
      }
    }
    // verify
    for (int i = 0; i < nodes; i ++) {
      PutLL(i, ret);
      Send(i);
    }
    count = 0;
    for (int i = 0; i < nodes; i ++) {
      Receive(i);
      count+= GetLL(i);
    }
    if (2 * count > n) { 
      // printf("%lld %lld\n", ret, count);
      printf("%lld\n", ret);
    } else {
      printf("NO WINNER\n");
    }
}

void worker() {
    llong avg = (n + nodes - 1) / nodes;
    llong cur = -1, count = 0;
    for (llong i = avg * id; i < n && i < avg * (id + 1); i++) { 
        llong v = GetVote(i);
        if (count ==0) {
          count = 1;
          cur = v;
        } else if (cur == v) {
          count ++;
        } else if (cur != v) {
          count --;
        }
    }
    PutLL(M, cur);
    PutLL(M, count);
    Send(M);
   
    // count
    Receive(M);
    cur = GetLL(M);
    count = 0;
    for (llong i = avg * id; i < n && i < avg * (id + 1); i++) { 
        llong v = GetVote(i);
        if (v == cur) count++;
    }
    PutLL(M, count);
    Send(M);
}

int main() {
    id = MyNodeId();
    n = GetN();
    nodes = NumberOfNodes() - 1;
    M = nodes;
    if (id == nodes) {
      master();
    } else {
      worker();
    } 
}
