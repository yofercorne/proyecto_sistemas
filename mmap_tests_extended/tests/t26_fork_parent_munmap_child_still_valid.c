uint64_t main() {
  uint64_t fd;
  uint64_t pid;
  uint64_t* p;
  uint64_t* status;

  fd = open("mmap_t26.bin", 2, 0);
  if (fd == 4294967295)
    exit(1);

  p = mmap(0, 4096, 2, fd, 0);

  status = malloc(sizeof(uint64_t));

  pid = fork();

  if (pid == 4294967295)
    exit(2);

  if (pid == 0) {
    if (*p != 2601)
      exit(3);

    *p = 2626;

    if (msync(p) != 0)
      exit(4);

    exit(0);
  } else {
    if (munmap(p) != 0)
      exit(5);

    wait(status);

    exit(0);
  }
}
