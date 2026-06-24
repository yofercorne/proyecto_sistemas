uint64_t *foo;

uint64_t main () {
	uint64_t waiter;
	foo = "Hello World!    ";

	waiter = 0;
	if (getpid() < 3) {
		waiter = 1;
	}
	futex_wait (waiter, 1);

	while (*foo != 0) {
		write (1, foo, 8);
		foo = foo + 1;
	}

	futex_wake (1);
}
