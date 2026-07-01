uint64_t main() {
  uint64_t fd;
  uint64_t r;
  uint64_t* p;

  fd = open("mmap_t24.bin", 2, 0);
  if (fd == 4294967295)
    exit(1);

  p = mmap(0, 8192, 2, fd, 0);

  if (*p != 2401)
    exit(2);

  if (*(p + 512) != 2402)
    exit(3);

  if (munmap(p) != 0)
    exit(4);

  r = msync(p);

  if (r != 4294967295)
    exit(5);

  exit(0);
}
