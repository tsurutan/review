CC := gcc
CFLAGS := -g -Wall -Wextra
level_rate: ./src/level_rate.c
	$(CC) ./src/csv.c ./src/level_rate.c -o ./output/level_rate $(CFLAGS)
test: ./src/test/csv_test.c
	$(RM) ./output/test_csv
	$(CC) ./src/csv.c ./src/test/csv_test.c -o ./output/test_csv $(CFLAGS)
	./output/test_csv
clear:
	$(RM) level_rate
