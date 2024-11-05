/* gcc -o standard-c -O3 -std=c11 -pedantic -Wall -Wextra -Werror -Wwrite-strings -Wconversion standard.c
*/

#include <stdio.h>
#include <stdlib.h>

#define BSIZE (64 * 1024)

void search(char b[], int n, int i) {
    if (i == n) {
        fwrite(b, 1, (size_t)n + 1, stdout);
    } else {
        for (char d = '0'; d <= '9'; ++d) {
            b[i] = d;
            search(b, n, i + 1);
        }
    }
}

int main() {
    char *buf = malloc(BSIZE);
    if (buf == NULL) {
        return 1;
    }

    if (setvbuf(stdout, buf, _IOFBF, BSIZE) != 0) {
        return 1;
    }

    int n;
    if (scanf("%d", &n) != 1) {
        return 1;
    }

    char *b = malloc((size_t)n + 1);
    if (b == NULL) {
        return 1;
    }
    b[n] = '\n';
    search(b, n, 0);
}
