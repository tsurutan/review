#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include "csv.h"
#include "level_struct.h"

/* 現実値 20190311 過去一ヶ月 */
const int LEVEL_SIZE = 10; // レベル数
const int REDUCTION = 55; // 商品の還元値

/* レベル比率・値はLevel2を基準とした倍率を表す
   参考: 時空物語外伝 イライザのゴールドラッシュ 経験値テーブル(Level10まで)
   https://taiyoproject.com/post-142 */
const float THRESHOLD_RATE_BY_LEVEL[LEVEL_SIZE] = {0, 1, 2.1, 3.3, 4.6, 6, 7.5, 9.2, 11.1, 13.2};

/* Faucet比率・値はLevel2を基準とした倍率を表す
   参考: 時空物語外伝 イライザのゴールドラッシュ 経験値テーブル(Level11まで)
   https://taiyoproject.com/post-142 */
const float FAUCET_REWARD_RATE_BY_LEVEL[LEVEL_SIZE] = {1, 2.1, 3.3, 4.6, 6, 7.5, 9.2, 11.1, 13.2, 15.5};


const int IDEAL_REDUCTION = 65; //目標還元値
const int ALLOWABLE_REDUCTION_ERROR = 2; // 目標還元値との許容誤差
const int ALLOWABLE_COMP_COUNT = 8; // 全レベルとの比較回数許容値

void calc_summary(int base_threshold, int base_faucet_reward);
void free_summary();
void init_level_info(LevelInfo *level_info, int index, int base_threshold, int base_faucet_reward);
void calc_level_info(Summary *summary);
int calc_expenses(ValueCount value_count, int reward);
long calc_sales(ValueCount people_count, int threshold);
int calc_reduction(long expenses, long sales);

int main() {
  open_level_info_csv();
  for(int base_threshold = 1000; base_threshold < 500000; base_threshold+=1000) {
    for(int base_faucet_reward = 1; base_faucet_reward < 100; base_faucet_reward+=1) {
      calc_summary(base_threshold, base_faucet_reward);
    }
  }
  close_csv();
}

void calc_summary(int base_threshold, int base_faucet_reward) {
  int comp = 0;
  // init summary
  Summary *entity;
  entity = (Summary*)malloc(sizeof(Summary));
  entity->revenue.expenses = 0;
  entity->revenue.sales = 0;
  entity->revenue.reduction = 0;
  entity->level_info = (LevelInfo*)malloc(sizeof(LevelInfo));
  entity->level_info->prev = NULL;

  for(int i = 0; i < LEVEL_SIZE; i++) {
    LevelInfo *target = entity->level_info;
    init_level_info(target, i, base_threshold, base_faucet_reward);
    calc_level_info(entity);

    if(target->prev != NULL && target->prev->revenue.reduction <= target->revenue.reduction) comp += 1;
    if(i == LEVEL_SIZE - 1) break;

    LevelInfo *next;
    next = (LevelInfo*)malloc(sizeof(LevelInfo));
    next->prev = target;
    entity->level_info = next;
  }

  // 還元値と理論値のギャップが許容範囲内か確認
  // 全レベルと比較して大になる許容範囲内か確認
  if(comp >= ALLOWABLE_COMP_COUNT && abs(entity->revenue.reduction - IDEAL_REDUCTION) <= ALLOWABLE_REDUCTION_ERROR) {
    write_summary_csv(base_threshold, base_faucet_reward, comp, *entity);
  }
  free_summary(entity);
}

void init_level_info(LevelInfo *level_info, int index, int base_threshold, int base_faucet_reward) {
  unsigned long threshold = base_threshold * THRESHOLD_RATE_BY_LEVEL[index];
  unsigned long next_threshold;
  if(index == LEVEL_SIZE - 1) {
    next_threshold = 0;
  } else {
    next_threshold = base_threshold * THRESHOLD_RATE_BY_LEVEL[index + 1];
  }
  level_info->threshold = threshold;
  level_info->people.all = calc_people_all_count_sum(threshold, next_threshold);
  level_info->people.invited = calc_people_invited_count_sum(threshold, next_threshold);
  level_info->people.non_invited = level_info->people.all - level_info->people.invited;
  level_info->faucet.all = calc_fc_all_count_sum(threshold, next_threshold);
  level_info->faucet.invited = calc_fc_invited_count_sum(threshold, next_threshold);
  level_info->faucet.non_invited = level_info->faucet.all - level_info->faucet.invited;
  level_info->faucet_reward = base_faucet_reward * FAUCET_REWARD_RATE_BY_LEVEL[index];
}

void calc_level_info(Summary *summary) {
  LevelInfo *target = summary->level_info;
  target->revenue.expenses = calc_expenses(target->people, target->threshold);
  target->revenue.expenses += calc_expenses(target->faucet, target->faucet_reward);
  target->revenue.sales = calc_sales(target->people, target->threshold);
  target->revenue.reduction = calc_reduction(target->revenue.expenses, target->revenue.sales);

  summary->revenue.expenses += target->revenue.expenses;
  summary->revenue.sales += target->revenue.sales;
  summary->revenue.reduction = calc_reduction(summary->revenue.expenses, summary->revenue.sales);
}

// 還元率の計算
int calc_reduction(long expenses, long sales) {
  if(sales == 0) {
    return 0;
  } else {
    return ((float) expenses / sales) * 100;
  }
}

// 支出の計算
int calc_expenses(ValueCount value_count, int reward) {
  return (value_count.non_invited + value_count.invited * 1.1) * reward;
}

// 売上の計算
long calc_sales(ValueCount people_count, int threshold) {
  return ((people_count.invited + people_count.non_invited) * threshold * 100) / REDUCTION;
}

void free_summary(Summary *summary) {
  LevelInfo *target = summary->level_info;
  LevelInfo *tmp;
  while(target != NULL) {
    tmp = target->prev;
    free(target);
    target = tmp;
  }
  free(summary);
}
