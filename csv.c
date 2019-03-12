#include "csv.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define BUFFER 512
#define LEVEL_SIZE 10

static FILE *f = NULL;

void concat_list(int list[LEVEL_SIZE], char output[BUFFER]) {
  sprintf(output, "%d,%d,%d,%d,%d,%d,%d,%d,%d,%d", list[0], list[1], list[2], list[3], list[4], list[5], list[6], list[7], list[8], list[9]);
}

void open_csv(char *file_path, char *opt) {
  f = fopen(file_path, opt);
}

void open_level_info_csv() {
  open_csv("output.csv", "w+");
  write_csv("Base閾値,Base報酬,総合還元率,前レベルより還元率が大きい回数,1還元率,2還元率,3還元率,4還元率,5還元率,6還元率,7還元率,8還元率,9還元率,10還元率,1閾値,2閾値,3閾値,4閾値,5閾値,6閾値,7閾値,8閾値,9閾値,10閾値,1報酬,2報酬,3報酬,4報酬,5報酬,6報酬,7報酬,8報酬,9報酬,10報酬\n\0");
}

void write_csv(char *buf) {
  fwrite(buf, strlen(buf), 1 , f);
}

void write_summary_csv(int threshold, int faucet_reward, int comp, summary entity) {
  char output[BUFFER] = "";
  int threshold_list[LEVEL_SIZE];
  int faucet_reward_list[LEVEL_SIZE];
  int reduction_list[LEVEL_SIZE];

  char threshold_val[BUFFER];
  char faucet_reward_val[BUFFER];
  char reduction_val[BUFFER];
  level_info *target = entity.level_info;

  for(int index = LEVEL_SIZE - 1; target; target=target->prev, index--) {
    threshold_list[index] = target->threshold;
    faucet_reward_list[index] = target->faucet_reward;
    reduction_list[index] = target->revenue.reduction;
  }
  concat_list(threshold_list, threshold_val);
  concat_list(faucet_reward_list, faucet_reward_val);
  concat_list(reduction_list, reduction_val);

  sprintf(output, "%d,%d,%d,%d,%s,%s,%s\n", threshold, faucet_reward, entity.revenue.reduction, comp, reduction_val, threshold_val, faucet_reward_val);
  write_csv(output);
}
void close_csv(void) {
  fclose(f);
}
