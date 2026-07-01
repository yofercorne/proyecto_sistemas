uint64_t main() {
  uint64_t fd;
  uint64_t* p0;
  uint64_t* p1;

  fd = open("mmap_t06.bin", 2, 0);
  if (fd == 4294967295)
    exit(1);

  p0 = mmap(0, 4096, 2, fd, 0);
  p1 = mmap(0, 4096, 2, fd, 4096);

  if (*p0 != 601)
    exit(2);

  if (*p1 != 602)
    exit(3);

  *p0 = 666;

  if (*p1 == 666)
    exit(4);

  exit(0);
}
