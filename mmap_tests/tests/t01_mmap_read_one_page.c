uint64_t main() {
  uint64_t fd;
  uint64_t* p;

  fd = open("mmap_t01.bin", 0, 0);
  if (fd == 4294967295)
    exit(1);

  p = mmap(0, 4096, 0, fd, 0);

  if (*p != 101)
    exit(2);

  if (*(p + 1) != 102)
    exit(3);

  exit(0);
}
