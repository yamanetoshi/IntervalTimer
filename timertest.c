#include <stdio.h>
#include <signal.h>
#include <sys/time.h>

static long int real_timer;

static void sig_handler(int signo) {
	switch (signo) {
	case SIGALRM:
		real_timer++;
		break;
	default:
		break;
	}
}

static long unsigned int fibonacci(unsigned int n) {
	if(n == 0)
		return 0;
	else if(n == 1 || n == 2)
		return 1;
	else
		return (fibonacci(n - 1) + fibonacci(n - 2));
}

int main(void) {
	int n = 45;
	long unsigned int ret;
	struct itimerval new, current;

	if(signal(SIGALRM, sig_handler) == SIG_ERR)
		fprintf(stderr, "Unable to create handler for SIGARM\n");

	new.it_interval.tv_sec = 1;
	new.it_interval.tv_usec = 0;
	new.it_value.tv_sec = 1;
	new.it_value.tv_usec = 0;
	setitimer(ITIMER_REAL, &new, NULL);

	ret = fibonacci(n);

	getitimer(ITIMER_REAL, &current);

	printf("fib(%d) = %ld\n", n, ret);
	printf("real time = %ld sec, %ld usec\n",
	       real_timer, current.it_value.tv_usec);

	signal(SIGALRM, SIG_DFL);

	return 0;
}
