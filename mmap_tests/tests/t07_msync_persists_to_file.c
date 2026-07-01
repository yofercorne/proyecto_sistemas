uint64_t main() {
  uint64_t fd;
  uint64_t r;
  uint64_t* p;

  fd = open("mmap_t07.bin", 2, 0);
  if (fd == 4294967295)
    exit(1);

  p = mmap(0, 4096, 2, fd, 0);

  if (*p != 701)
    exit(2);

  *p = 777;

  r = msync(p);
  if (r != 0)
    exit(3);

  exit(0);
}