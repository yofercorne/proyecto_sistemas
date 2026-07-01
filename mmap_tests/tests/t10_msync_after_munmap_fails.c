uint64_t main() {
  uint64_t fd;
  uint64_t r;
  uint64_t* p;

  fd = open("mmap_t10.bin", 2, 0);
  if (fd == 4294967295)
    exit(1);

  p = mmap(0, 4096, 2, fd, 0);

  r = munmap(p);
  if (r != 0)
    exit(2);

  r = msync(p);

  if (r != 4294967295)
    exit(3);

  exit(0);
}
