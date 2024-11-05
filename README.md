# c-go-stdout-performance

Этот репозиторий связан с вопросом [Go обгоняет C на задаче с объёмным выводом](https://ru.stackoverflow.com/q/1598560/416121) на https://ru.stackoverflow.com/.

## Вопрос

Решалась задача вывода последовательностей вида: `000 001 002 ... 997 998 999`.

Были написаны два решения на Go и на C: [standard.go](https://github.com/StanislavVolodarskiy/c-go-stdout-performance/blob/main/standard.go), [standard.c](https://github.com/StanislavVolodarskiy/c-go-stdout-performance/blob/main/standard.c). Оба решения использовали буфер *64KB* для `stdout`. Оба решения написаны с использованием стандартных средств языков и библиотек. Оказалось что решение на Go заметно опережает решение на C.

Время работы программы на Go на четверть меньше такого же времени для программы на C:

>     $ make standard-go standard-c
>     go build -o standard-go standard.go
>     gcc -o standard-c -O3 -std=c11 -pedantic -Wall -Wextra -Werror -Wwrite-strings -Wconversion standard.c
>     
>     $ time -p echo 8 | ./standard-c | wc -l
>     100000000
>     real 2.12
>     user 2.86
>     sys 0.24
>     
>     $ time -p echo 8 | ./standard-go | wc -l
>     100000000
>     real 1.51
>     user 2.24
>     sys 0.18
>
>     $ time -p echo 9 | ./standard-c | wc -l
>     1000000000
>     real 21.65
>     user 29.50
>     sys 2.60
>     
>     $ time -p echo 9 | ./standard-go | wc -l
>     1000000000
>     real 14.74
>     user 21.73
>     sys 2.44

Собственно вопрос: почему так происходит? Почему стандартный код на C медленее станартного кода на Go?

## Ответ

