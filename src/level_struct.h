#ifndef LEVEL_STRUCT
#define LEVEL_STRUCT

typedef struct _value_count {
  unsigned long non_invited;
  unsigned long invited;
} value_count;

typedef struct _revenue {
  int reduction;
  unsigned long expenses;
  unsigned long sales;
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

#endif
