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

С оказался медленнее Go из-за особенностей стандартной библиотеки.

В C буфер для файла встроен в сам файл и функция `fwrite` по стандарту обязана работать в многопоточной среде.  Чтобы не допускать гонку `fwrite` использует блокировку ([пример исходного кода](https://github.com/bminor/glibc/blob/master/libio/iofwrite.c#L37-L40)). Если мы вызываем её миллиард раз, мы миллиард раз блокируем файл, что требует времени.

В Go буфер &ndash; отдельный объект, который не обязан работать в многопоточной среде. Вызов `buffer.Write` ничего не блокирует, только переписывает данные в память буфера. Когда буфер заполняется, выполняется запись в файл и эта запись синхронная, то есть какая-то блокировка есть. Но в случае с C блокировка вызывается каждые десять байт (это длина одной строки), а в случае с Go &ndash; каждые *64KB*, то есть в шесть с половиной тысяч раз реже. Что позволяет Go обойти C.

Время работы программы на Go на четверть больше такого же времени для программы на C с буфером:

>     $ time -p echo 8 | ./standard-go | wc -l
>     100000000
>     real 1.45
>     user 2.09
>     sys 0.22
>     
>     $ time -p echo 8 | ./buffered-c | wc -l
>     100000000
>     real 1.01
>     user 1.56
>     sys 0.28
>     
>     $ time -p echo 9 | ./standard-go | wc -l
>     1000000000
>     real 15.72
>     user 23.69
>     sys 2.40
>     
>     $ time -p echo 9 | ./buffered-c | wc -l
>     1000000000
>     real 11.27
>     user 17.74
>     sys 3.58

