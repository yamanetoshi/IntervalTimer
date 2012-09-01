#include <stdio.h>
#include <signal.h>
#include <sys/time.h>

static long int real_timer, virtual_timer, prof_timer;

static void sig_handler(int signo) {
	switch (signo) {
	case SIGALRM:
		real_timer++;
		break;
	case SIGVTALRM:
		virtual_timer++;
		break;
	case SIGPROF:
		prof_timer++;
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
	struct itimerval realt, virtt, proft, current_r, current_v, current_p;

	if(signal(SIGALRM, sig_handler) == SIG_ERR)
		fprintf(stderr, "Unable to create handler for SIGARM\n");

	if(signal(SIGVTALRM, sig_handler) == SIG_ERR)
		fprintf(stderr, "Unable to create handler for SIGARM\n");

	if(signal(SIGPROF, sig_handler) == SIG_ERR)
		fprintf(stderr, "Unable to create handler for SIGARM\n");

	realt.it_interval.tv_sec = 1;
	realt.it_interval.tv_usec = 0;
	realt.it_value.tv_sec = 1;
	realt.it_value.tv_usec = 0;
	setitimer(ITIMER_REAL, &realt, NULL);

	virtt.it_interval.tv_sec = 1;
	virtt.it_interval.tv_usec = 0;
	virtt.it_value.tv_sec = 1;
	virtt.it_value.tv_usec = 0;
	setitimer(ITIMER_VIRTUAL, &virtt, NULL);

	proft.it_interval.tv_sec = 1;
	proft.it_interval.tv_usec = 0;
	proft.it_value.tv_sec = 1;
	proft.it_value.tv_usec = 0;
	setitimer(ITIMER_PROF, &proft, NULL);

	ret = fibonacci(n);

	getitimer(ITIMER_REAL, &current_r);
	getitimer(ITIMER_VIRTUAL, &current_v);
	getitimer(ITIMER_PROF, &current_p);

	printf("fib(%d) = %ld\n", n, ret);
	printf("real time   = %3ld sec, %3ld msec\n",
	       real_timer, 1000 - (current_r.it_value.tv_usec/1000));
	printf("cpu time    = %3ld sec, %3ld msec\n",
	       prof_timer, 1000 - (current_p.it_value.tv_usec/1000));
	printf("user time   = %3ld sec, %3ld msec\n",
	       virtual_timer, 1000 - (current_v.it_value.tv_usec/1000));
	printf("kernel time = %3ld sec, %3ld msec\n",
	       prof_timer - virtual_timer, 
	       (1000 - (current_p.it_value.tv_usec/1000)) - 
	       (1000 - (current_v.it_value.tv_usec/1000)));

	signal(SIGALRM, SIG_DFL);
	signal(SIGVTALRM, SIG_DFL);
	signal(SIGPROF, SIG_DFL);

	return 0;
}
