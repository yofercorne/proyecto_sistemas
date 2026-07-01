uint64_t main() {
  uint64_t fd;
  uint64_t fd2;
  uint64_t pid;
  uint64_t* p;
  uint64_t* b;
  uint64_t* status;

  fd = open("mmap_t12.bin", 2, 0);
  if (fd == 4294967295)
    exit(1);

  p = mmap(0, 4096, 2, fd, 0);

  if (*p != 1201)
    exit(2);

  status = malloc(sizeof(uint64_t));

  pid = fork();

  if (pid == 4294967295)
    exit(3);

  if (pid == 0) {
    *p = 1222;

    if (msync(p) != 0)
      exit(4);

    exit(0);
  } else {
    wait(status);

    b = malloc(sizeof(uint64_t));

    fd2 = open("mmap_t12.bin", 0, 0);
    if (fd2 == 4294967295)
      exit(5);

    read(fd2, b, 8);

    if (*b != 1222)
      exit(6);

    exit(0);
  }
}
