uint64_t main() {
  uint64_t fd;
  uint64_t* p;

  fd = open("mmap_t03.bin", 0, 0);
  if (fd == 4294967295)
    exit(1);

  // length = 1 debe redondearse internamente a 4096.
  p = mmap(0, 1, 0, fd, 0);

  if (*p != 301)
    exit(2);

  // Última palabra de la primera página: byte offset 4088.
  if (*(p + 511) != 399)
    exit(3);

  exit(0);
}
