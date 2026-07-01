uint64_t main() {
  uint64_t fd;
  uint64_t* p;

  fd = open("mmap_t17.bin", 2, 0);
  if (fd == 4294967295)
    exit(1);

  p = mmap(0, 4096, 0, fd, 0);

  *p = 1717;

  exit(0);
}
