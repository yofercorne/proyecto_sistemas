uint64_t main() {
  uint64_t fd;
  uint64_t pid;
  uint64_t* p;
  uint64_t* status;

  status = malloc(sizeof(uint64_t));

  pid = fork();

  if (pid == 4294967295)
    exit(1);

  if (pid == 0) {
    fd = open("mmap_t27.bin", 2, 0);
    if (fd == 4294967295)
      exit(2);

    p = mmap(0, 4096, 2, fd, 0);

    if (*p != 2701)
      exit(3);

    *p = 2777;

    exit(0);
  } else {
    wait(status);

    fd = open("mmap_t27.bin", 2, 0);
    if (fd == 4294967295)
      exit(4);

    p = mmap(0, 4096, 2, fd, 0);

    if (*p != 2777)
      exit(5);

    exit(0);
  }
}
