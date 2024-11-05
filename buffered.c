/* gcc -o buffered-c -O3 -std=c11 -pedantic -Wall -Wextra -Werror -Wwrite-strings -Wconversion -I. buffer_t.c buffered.c
*/

#include <stdio.h>
#include <stdlib.h>

#include <buffer_t.h>

#define BSIZE (64 * 1024)

static buffer_t bufout;

static void search(char b[/* n + 1 */], int n, int i) {
    if (i == n) {
        bwrite(&bufout, (size_t)n + 1, b);
    } else {
        for (char d = '0'; d <= '9'; ++d) {
            b[i] = d;
            search(b, n, i + 1);
        }
    }
}

int main() {
    if (!binit(&bufout, BSIZE, stdout)) {
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

    bflush(&bufout);
    bterm(&bufout);
}
