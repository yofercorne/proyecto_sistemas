uint64_t main() {
  uint64_t fd1;
  uint64_t fd2;
  uint64_t* p1;
  uint64_t* p2;

  fd1 = open("mmap_t28a.bin", 2, 0);
  if (fd1 == 4294967295)
    exit(1);

  fd2 = open("mmap_t28b.bin", 2, 0);
  if (fd2 == 4294967295)
    exit(2);

  p1 = mmap(0, 4096, 2, fd1, 0);
  p2 = mmap(0, 4096, 2, fd2, 0);

  if (*p1 != 2801)
    exit(3);

  if (*p2 != 2802)
    exit(4);

  *p1 = 2828;

  if (*p2 == 2828)
    exit(5);

  if (*p2 != 2802)
    exit(6);

  exit(0);
}
