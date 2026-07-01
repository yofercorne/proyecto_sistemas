uint64_t main() {
  uint64_t fd;
  uint64_t* p;

  fd = open("mmap_t23.bin", 2, 0);
  if (fd == 4294967295)
    exit(1);

  p = mmap(0, 8192, 2, fd, 0);

  if (*p != 2301)
    exit(2);

  if (*(p + 512) != 2302)
    exit(3);

  *p = 2323;
  *(p + 512) = 2424;

  if (msync(p) != 0)
    exit(4);

  exit(0);
}
