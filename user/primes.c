#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void eliminate(int p, int r, int w) {
	int n;
	while (read(r, &n, sizeof(n))) {
		if (n % p != 0) {
			write(w, &n, sizeof(n));
		}
	}
}

void right(int r) {
	int p;
	if (read(r, &p, sizeof(p))) {
		printf("prime %d\n", p);
		int pd[2];
		pipe(pd);
		if (fork()) {
			close(pd[1]);
			right(pd[0]);
		}
		else {
			close(pd[0]);
			eliminate(p, r, pd[1]);
			close(pd[1]);
		}
	}
}

int main(int argc, char **argv) {
	int pd[2];
	pipe(pd);
	if (fork()) {
		close(pd[1]);
		right(pd[0]);
	}
	else {
		close(pd[0]);
        for (int i = 2; i < 36; i++) {
            write(pd[1], &i, sizeof(i));
        }
        close(pd[1]);
	}
	exit(0);
}