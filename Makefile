CC := gcc
CFLAGS := -g -Wall -Wextra
main: ./src/main.c
	$(RM) main
	$(CC) ./src/csv.c ./src/main.c -o ./output/main $(CFLAGS)
test: ./src/test/csv_test.c
	$(RM) ./output/test_csv
	$(CC) ./src/csv.c ./src/test/csv_test.c -o ./output/test_csv $(CFLAGS)
	./output/test_csv
run:
	./output/main
	open ./output/result.csv
clear:
	$(RM) main
