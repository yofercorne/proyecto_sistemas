uint64_t main() {
  uint64_t fd;
  uint64_t* p;

  fd = open("mmap_t02.bin", 0, 0);
  if (fd == 4294967295)
    exit(1);

  p = mmap(0, 4096, 0, fd, 4096);

  if (*p != 202)
    exit(2);

  exit(0);
}
