uint64_t main() {
  uint64_t fd;
  uint64_t pid;
  uint64_t* p;
  uint64_t* status;

  fd = open("mmap_t25.bin", 2, 0);
  if (fd == 4294967295)
    exit(1);

  p = mmap(0, 4096, 2, fd, 0);

  status = malloc(sizeof(uint64_t));

  pid = fork();

  if (pid == 4294967295)
    exit(2);

  if (pid == 0) {
    if (munmap(p) != 0)
      exit(3);

    exit(0);
  } else {
    wait(status);

    if (*p != 2501)
      exit(4);

    *p = 2525;

    if (msync(p) != 0)
      exit(5);

    exit(0);
  }
}
