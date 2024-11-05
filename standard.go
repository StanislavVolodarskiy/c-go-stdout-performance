package main

import (
    "bufio"
    "fmt"
    "os"
)

func main() {
    var n int
    fmt.Scan(&n)

    buffer := bufio.NewWriterSize(os.Stdout, 64 * 1024)

    b := make([]byte, n+1)
    b[n] = '\n'

    var search func(i int)
    search = func(i int) {
        if i == n {
            buffer.Write(b)
        } else {
            for d := '0'; d <= '9'; d++ {
                b[i] = byte(d)
                search(i + 1)
            }
        }
    }
    search(0);
    buffer.Flush()
}
