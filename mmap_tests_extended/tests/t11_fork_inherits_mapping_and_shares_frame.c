uint64_t main() {
  uint64_t fd;
  uint64_t pid;
  uint64_t* p;
  uint64_t* status;

  fd = open("mmap_t11.bin", 2, 0);
  if (fd == 4294967295)
    exit(1);

  p = mmap(0, 4096, 2, fd, 0);

  if (*p != 1101)
    exit(2);

  status = malloc(sizeof(uint64_t));

  pid = fork();

  if (pid == 4294967295)
    exit(3);

  if (pid == 0) {
    *p = 1111;
    exit(0);
  } else {
    wait(status);

    if (*p != 1111)
      exit(4);

    exit(0);
  }
}
