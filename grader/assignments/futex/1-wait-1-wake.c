uint64_t *foo;

uint64_t main () {
	foo = "Hello World!    ";

	futex_wait (getpid(), 1);

	while (*foo != 0) {
		write (1, foo, 8);
		foo = foo + 1;
	}

	futex_wake (1);
}
