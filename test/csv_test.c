#include <stdio.h>
#include <assert.h>
#include "../csv.h"

int main() {
  int i = 10;
  open_csv("test.csv", "r");
  assert(i == 10);
}