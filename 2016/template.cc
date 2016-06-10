#include <stdio.h>
#include <string.h>
using namespace std;

#include <message.h>

#define llong long long

#include "majority.h"

int id, nodes, M;
llong n;

void master() {
}

void worker() {
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
