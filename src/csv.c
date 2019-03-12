#include "csv.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define BUFFER 512
#define LEVEL_SIZE 10
#define INPUT_SIZE 1000

static FILE *f = NULL;
static Data fc_all[INPUT_SIZE];
static Data fc_invited[INPUT_SIZE];
static ValueCount fcCount;
static Data people_all[INPUT_SIZE];
static Data people_invited[INPUT_SIZE];
static ValueCount peopleCount;

void concat_list(int list[LEVEL_SIZE], char output[BUFFER]) {
  sprintf(output, "%d,%d,%d,%d,%d,%d,%d,%d,%d,%d", list[0], list[1], list[2], list[3], list[4], list[5], list[6], list[7], list[8], list[9]);
}

int read_file(char *file_path, Data *data) {
  FILE *data_fp;
  data_fp = fopen(file_path, "r");
  char output[BUFFER];
  char *ptr;
  int is_header = 1;
  int index = 0;
  while(fgets(output, BUFFER, data_fp) != NULL) {
    if(is_header) {
      is_header = 0;
    } else {
      Data datum;
      ptr = strtok(output, ",");
      datum.lower_limit = atoi(strtok(NULL, ","));
      datum.upper_limit = atoi(strtok(NULL, ","));
      datum.count = atoi(strtok(NULL, ","));
      data[index] = datum;
      index++;
    }
  }
  return index - 1;
}

unsigned long calc_count_sum(int total_count, Data *data, unsigned long threshold, unsigned long next_threshold) {
  int lower_pos, upper_pos;
  unsigned long sum = 0;
  for(lower_pos = 0; lower_pos < INPUT_SIZE; lower_pos++) {
    if(data[lower_pos].upper_limit >= threshold) break;
  }
  if(next_threshold == 0) {
    upper_pos = total_count;
  } else {
    for(upper_pos = 0; upper_pos < INPUT_SIZE; upper_pos++) {
      if(data[upper_pos].upper_limit >= next_threshold) break;
    }
  }
  for(int index = lower_pos; index <= upper_pos; index++) {
    sum += data[index].count;
  }
  return sum;
}

unsigned long calc_fc_all_count_sum(unsigned long threshold, unsigned long next_threshold) {
  return calc_count_sum(fcCount.all, fc_all, threshold, next_threshold);
}

unsigned long calc_fc_invited_count_sum(unsigned long threshold, unsigned long next_threshold) {
  return calc_count_sum(fcCount.invited, fc_invited, threshold, next_threshold);
}

unsigned long calc_people_all_count_sum(unsigned long threshold, unsigned long next_threshold) {
  return calc_count_sum(peopleCount.all, people_all, threshold, next_threshold);
}

unsigned long calc_people_invited_count_sum(unsigned long threshold, unsigned long next_threshold) {
  return calc_count_sum(peopleCount.invited, people_invited, threshold, next_threshold);
}

void open_csv(char *file_path, char *opt) {
  f = fopen(file_path, opt);
}

void open_level_info_csv() {
  open_csv("./output/result.csv", "w+");
  write_csv("Base閾値,Base報酬,総合還元率,前レベルより還元率が大きい回数,総合支出,総合売上,1還元率,2還元率,3還元率,4還元率,5還元率,6還元率,7還元率,8還元率,9還元率,10還元率,1閾値,2閾値,3閾値,4閾値,5閾値,6閾値,7閾値,8閾値,9閾値,10閾値,1報酬,2報酬,3報酬,4報酬,5報酬,6報酬,7報酬,8報酬,9報酬,10報酬,1人数,2人数,3人数,4人数,5人数,6人数,7人数,8人数,9人数,10人数,1支出,2支出,3支出,4支出,5支出,6支出,7支出,8支出,9支出,10支出,1売上,2売上,3売上,4売上,5売上,6売上,7売上,8売上,9売上,10売上\n\0");
  fcCount.all = read_file("./input/fc_all_2500.csv", fc_all);
  fcCount.invited = read_file("./input/fc_invited_2500.csv", fc_invited);
  peopleCount.all = read_file("./input/people_all_2500.csv", people_all);
  peopleCount.invited = read_file("./input/people_invited_2500.csv", people_invited);
}

void write_csv(char *buf) {
  fwrite(buf, strlen(buf), 1 , f);
}

//結果をCSVへ書き込む
void write_summary_csv(int threshold, int faucet_reward, int comp, Summary entity) {
  char output[BUFFER] = "";
  int threshold_list[LEVEL_SIZE];
  int faucet_reward_list[LEVEL_SIZE];
  int reduction_list[LEVEL_SIZE];
  int people_list[LEVEL_SIZE];
  int expenses_list[LEVEL_SIZE];
  int sales_list[LEVEL_SIZE];

  char threshold_val[BUFFER];
  char faucet_reward_val[BUFFER];
  char reduction_val[BUFFER];
  char people_val[BUFFER];
  char expenses_val[BUFFER];
  char sales_val[BUFFER];
  LevelInfo *target = entity.level_info;

  for(int index = LEVEL_SIZE - 1; target; target=target->prev, index--) {
    threshold_list[index] = target->threshold;
    faucet_reward_list[index] = target->faucet_reward;
    reduction_list[index] = target->revenue.reduction;
    people_list[index] = target->people.all;
    expenses_list[index] = target->revenue.expenses;
    sales_list[index] = target->revenue.sales;
  }
  concat_list(threshold_list, threshold_val);
  concat_list(faucet_reward_list, faucet_reward_val);
  concat_list(reduction_list, reduction_val);
  concat_list(people_list, people_val);
  concat_list(expenses_list, expenses_val);
  concat_list(sales_list, sales_val);

  sprintf(output, "%d,%d,%d,%d,%lu,%lu,%s,%s,%s,%s,%s,%s\n", threshold, faucet_reward, comp, entity.revenue.reduction, entity.revenue.expenses, entity.revenue.sales, reduction_val, threshold_val, faucet_reward_val, people_val, expenses_val, sales_val);
  write_csv(output);
}

void close_csv(void) {
  fclose(f);
}
