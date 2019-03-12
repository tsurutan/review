#include <stdio.h>
#include <assert.h>
#include "./../csv.h"


int main() {
  static Data data[1000];
  int count = read_file("./src/test/test.csv", data);

  assert(data[0].lower_limit == 0);
  assert(data[0].upper_limit == 117);
  assert(data[0].count == 121008);

  assert(data[22].lower_limit == 7139);
  assert(data[22].upper_limit == 7256);
  assert(data[22].count == 345);

  assert(calc_count_sum(count, data, 0, 117) == 121008);
  assert(calc_count_sum(count, data, 0, 118) == 121827);
  assert(calc_count_sum(count, data, 118, 118) == 819);
  assert(calc_count_sum(count, data, 118, 1638) == 3679);
}
