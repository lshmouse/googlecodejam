#include <stdio.h>
#include <algorithm>
#include <message.h>

using namespace std;

#define llong long long

#include "sandwich.h"

int id, nodes, M;

void master() {
  llong total = 0, left = 0, right = 0, div = 0;
  for (int i = 0; i < nodes; i++) {
     Receive(i);
     llong total1 = GetLL(i);
     llong left1 = GetLL(i);
     llong right1 = GetLL(i);
     llong div1 = GetLL(i);
     // merge
     div = max(div + total1, total + div1);
     div = max(div, left);
     div = max(div, total + left1);
     div = max(div, left + right1);
     
     left = max(left, total + left1);
     right = max(right + total1, right1);
     total = total + total1;
  }
  div = max(div, right);
  div = max(div, left);
  div = max(div, total);
  printf("%lld\n", div);
}

void worker() {
  int n = GetN();
  int avg = (n + nodes - 1) / nodes;
  llong total = 0;
  int start = avg *id;
  int sz = max(0, min(n, avg * (id + 1)) - start);
  llong* values = new llong[sz];
  llong* left = new llong[sz + 1];
  llong* right = new llong[sz + 1];

  for (int i = 0 ; i < sz ; i ++) {
    values[i] = GetTaste(start + i);
    total += values[i];
  }

  left[0] = 0;
  llong sum = 0;
  for (int i = 0; i < sz; i++) {
     sum += values[i];
     left[i+1] = max(left[i], sum);
  }

  right[sz] = 0;
  sum = 0;
  for (int i = sz-1; i >=0; i--) {
     sum += values[i];
     right[i] = max(right[i + 1], sum);
  }
  llong div = total;
  for (int i = 0; i < sz + 1; i++) { 
    if (left[i] + right[i] > div) {
      div = left[i] + right[i];
    }
  }

  // printf("%d %lld %lld %lld %lld\n", id, total, left[sz], right[0], div);
  PutLL(M, total);
  PutLL(M, left[sz]);
  PutLL(M, right[0]);
  PutLL(M, div);
  Send(M);
  delete[] values;
  delete[] left;
  delete[] right;
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
