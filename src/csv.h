#include "level_struct.h"
void open_csv(char *file_path, char *opt);
void open_level_info_csv();
void write_csv(char *buf);
void close_csv(void);
void write_summary_csv(int threshold, int faucet_reward, int comp, Summary summary);
