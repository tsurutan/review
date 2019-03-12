#include "level_struct.h"
void open_csv(char *file_path, char *opt);
void open_level_info_csv();
void write_csv(char *buf);
void close_csv(void);
void write_summary_csv(int threshold, int faucet_reward, int comp, Summary summary);
int read_file(char *file_path, Data *data);
unsigned long calc_fc_all_count_sum(unsigned long threshold, unsigned long next_threshold);
unsigned long calc_fc_invited_count_sum(unsigned long threshold, unsigned long next_threshold);
unsigned long calc_people_all_count_sum(unsigned long threshold, unsigned long next_threshold);
unsigned long calc_people_invited_count_sum(unsigned long threshold, unsigned long next_threshold);
unsigned long calc_count_sum(int total_count, Data *data, unsigned long threshold, unsigned long next_threshold);
