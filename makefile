.PHONY: help
help:
	cat makefile

.PHONY: clean
clean:
	rm -f standard-c buffered-c standard-go

standard-c: standard.c
	gcc -o standard-c -O3 -std=c11 -pedantic -Wall -Wextra -Werror -Wwrite-strings -Wconversion standard.c

buffered-c: buffer_t.h buffer_t.c buffered.c
	gcc -o buffered-c -O3 -std=c11 -pedantic -Wall -Wextra -Werror -Wwrite-strings -Wconversion -I. buffer_t.c buffered.c

standard-go: standard.go
	go build -o standard-go standard.go

.PHONY: test
test: standard-c buffered-c standard-go
	bash -c "diff <(echo 8 | ./standard-c) <(echo 8 | ./buffered-c)"
	bash -c "diff <(echo 8 | ./buffered-c) <(echo 8 | ./standard-go)"

.PHONY: benchmark
benchmark: standard-c buffered-c standard-go test
	bash -c "time -p echo 8 | ./standard-c | wc -l"
	bash -c "time -p echo 8 | ./buffered-c | wc -l"
	bash -c "time -p echo 8 | ./standard-go | wc -l"
	bash -c "time -p echo 9 | ./standard-c | wc -l"
	bash -c "time -p echo 9 | ./buffered-c | wc -l"
	bash -c "time -p echo 9 | ./standard-go | wc -l"
