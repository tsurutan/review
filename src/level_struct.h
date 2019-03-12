#ifndef LEVEL_STRUCT
#define LEVEL_STRUCT

typedef struct _ValueCount {
  unsigned long non_invited;
  unsigned long invited;
} ValueCount;

typedef struct _Revenue {
  int reduction;
  unsigned long expenses;
  unsigned long sales;
} Revenue;

typedef struct _LevelInfo {
  ValueCount people;
  ValueCount faucet;
  unsigned long threshold;
  unsigned long faucet_reward;
  Revenue revenue;
  struct _LevelInfo *prev;
} LevelInfo;

typedef struct _Summary {
  LevelInfo *level_info;
  Revenue revenue;
} Summary;

#endif
