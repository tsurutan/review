#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdarg.h>

/* 現実値 20190311 過去一ヶ月 */
const int LEVEL_SIZE = 10; // レベル数
const int ENTIRE_PEOPLE_COUNT_BY_LEVEL[LEVEL_SIZE] = {4135, 174, 299, 58, 34, 25, 23, 9, 4, 1}; // レベルごと全体人数
const int INVITED_PEOPLE_COUNT_BY_LEVEL[LEVEL_SIZE] = {2447, 120, 217, 43, 24, 19, 17, 5, 4, 0}; // レベルごと被招待者人数
const int ENTIRE_FAUCET_COUNT_BY_LEVEL[LEVEL_SIZE] = {141644, 31255, 73496, 14274, 10160, 7806, 5890, 3576, 1959, 498}; // レベルごとFaucet回数
const int INVITED_FAUCET_COUNT_BY_LEVEL[LEVEL_SIZE] = {103175, 23654, 52403, 10637, 8371, 5075, 3666, 2478, 1959, 0}; // レベルごと被招待者Faucet回数
const int REDUCTION = 55; // 商品の還元値

/* レベル比率・値はLevel2を基準とした倍率を表す
   参考: 時空物語外伝 イライザのゴールドラッシュ 経験値テーブル(Level10まで)
   https://taiyoproject.com/post-142 */
const float THRESHOLD_RATE_BY_LEVEL[LEVEL_SIZE] = {0, 1, 2.1, 3.3, 4.6, 6, 7.5, 9.2, 11.1, 13.2};

/* レベル比率・値はLevel2を基準とした倍率を表す
   参考: 時空物語外伝 イライザのゴールドラッシュ 経験値テーブル(Level10まで)
   https://taiyoproject.com/post-142 */
const float FAUCET_REWARD_RATE_BY_LEVEL[LEVEL_SIZE] = {1, 2.5, 2.8, 3.0, 3.3, 3.8, 4.4, 5.0, 6.9, 8.1};

const int IDEAL_REDUCTION = 65; //目標還元値
const int ALLOWABLE_REDUCTION_ERROR = 1; // 目標還元値との許容誤差

typedef struct _value_count {
  unsigned long non_invited;
  unsigned long invited;
} value_count;

typedef struct _revenue {
  unsigned long expenses;
  unsigned long sales;
  int reduction;
} revenue;

typedef struct _level_info {
  value_count people;
  value_count faucet;
  unsigned long threshold;
  unsigned long faucet_reward;
  revenue revenue;
  struct _level_info *prev;
} level_info;

typedef struct _summary {
  level_info *level_info;
  revenue revenue;
} summary;


void calc_summary(int base_threshold, int base_faucet_reward);
void free_summary();
void init_level_info(level_info *level_info, int index, int base_threshold, int base_faucet_reward);
void calc_level_info(summary *summary);
int calc_expenses(value_count value_count, int reward);
long calc_sales(value_count people_count, int threshold);
int calc_reduction(long expenses, long sales);

// Debug
void debug_printf(const char *format, ...);
int is_debug = 0;

int main(int argc, char **argv) {
  int opt;
  while((opt = getopt(argc, argv, "dt:")) != -1) {
    switch(opt) {
      case 'd':
        is_debug = 1;
        break;
      default:
        break;
    }
  }

  for(int base_threshold = 1; base_threshold < 30000; base_threshold++) {
    for(int base_faucet_reward = 1; base_faucet_reward < 1000; base_faucet_reward+=5) {
      calc_summary(base_threshold, base_faucet_reward);
    }
  }
}

void calc_summary(int base_threshold, int base_faucet_reward) {
  int is_valid = 1;
  // init summary
  summary *summary = malloc(sizeof(summary));
  summary->revenue.expenses = 0;
  summary->revenue.sales = 0;
  summary->revenue.reduction = 0;
  summary->level_info = malloc(sizeof(level_info));
  summary->level_info->prev = NULL;

  int i;
  for(i = 0; i < LEVEL_SIZE; i++) {
    level_info *target = summary->level_info;
    init_level_info(target, i, base_threshold, base_faucet_reward);
    calc_level_info(summary);

    if(i == LEVEL_SIZE - 1) break;
    if(target->prev != NULL && target->prev->revenue.reduction > target->revenue.reduction) {
      printf("index = %d, prev reduction = %d, current reduction = %d\n", i, target->prev->revenue.reduction, target->revenue.reduction);
      is_valid = 0;
      return;
    };

    level_info *next = malloc(sizeof(level_info));
    next->prev = malloc(sizeof(target));
    next->prev = target;
    summary->level_info = next;
  }

  // 還元値と理論値のギャップが許容範囲内か確認
  if(abs(summary->revenue.reduction - IDEAL_REDUCTION) <= 2) {
    if(is_valid) {
      printf("Total\n");
      /* printf("支出 = %lu\n", summary->revenue.expenses); */
      /* printf("収入 = %lu\n", summary->revenue.sales); */
      printf("基本Faucet = %d\n", base_faucet_reward);
      printf("基本閾値 = %d\n", base_threshold);
      printf("還元値 = %d\n\n", summary->revenue.reduction);
    } else {
      level_info *target = summary->level_info;
    }

    level_info *target = summary->level_info;
    /* for(; target; target=target->prev) { */
    /*   printf("支出 = %lu\n", target->revenue.expenses); */
    /*   printf("収入 = %lu\n", target->revenue.sales); */
    /*   printf("還元率 = %d\n\n", target->revenue.reduction); */
    /*   printf("============\n"); */
    /* } */
    /* printf("is valid = %d\n", is_valid); */
  }
  free_summary(summary);
}

void free_summary(summary *summary) {
  level_info *target = summary->level_info;
  level_info *tmp;
  while(target != NULL) {
    tmp = target->prev;
    free(target);
    target = tmp;
  }
  free(summary);
}

void init_level_info(level_info *level_info, int index, int base_threshold, int base_faucet_reward) {
  level_info->people.invited = INVITED_PEOPLE_COUNT_BY_LEVEL[index];
  level_info->people.non_invited = ENTIRE_PEOPLE_COUNT_BY_LEVEL[index] - level_info->people.invited;
  level_info->faucet.invited = INVITED_FAUCET_COUNT_BY_LEVEL[index];
  level_info->faucet.non_invited = ENTIRE_FAUCET_COUNT_BY_LEVEL[index] - level_info->faucet.invited;
  level_info->faucet_reward = base_faucet_reward * FAUCET_REWARD_RATE_BY_LEVEL[index];
  level_info->threshold = base_threshold * THRESHOLD_RATE_BY_LEVEL[index];
}

void calc_level_info(summary *summary) {
  level_info *target = summary->level_info;
  target->revenue.expenses = calc_expenses(target->people, target->threshold);
  target->revenue.expenses += calc_expenses(target->faucet, target->faucet_reward);
  target->revenue.sales = calc_sales(target->people, target->threshold);
  target->revenue.reduction = calc_reduction(target->revenue.expenses, target->revenue.sales);

  summary->revenue.expenses += target->revenue.expenses;
  summary->revenue.sales += target->revenue.sales;
  summary->revenue.reduction = calc_reduction(summary->revenue.expenses, summary->revenue.sales);
  debug_printf("summary->level_info expenses = %lu\n", summary->revenue.expenses);
}

int calc_reduction(long expenses, long sales) {
  if(sales == 0) {
    return 0;
  } else {
    return ((float) expenses / sales) * 100;
  }
}

int calc_expenses(value_count value_count, int reward) {
  return (value_count.non_invited + value_count.invited * 1.1) * reward;
}

long calc_sales(value_count people_count, int threshold) {
  return ((people_count.invited + people_count.non_invited) * threshold * 100) / REDUCTION;
}

void debug_printf(const char *format, ...) {
  if(!is_debug) return;
  va_list ap;
  va_start(ap, format);
  vprintf(format, ap);
  va_end(ap);
}
