CC := gcc
CFLAGS := -g -Wall -Wextra
level_rate: level_rate.c
	$(CC) csv.c level_rate.c -o level_rate $(CFLAGS)
clear:
	$(RM) level_rate
