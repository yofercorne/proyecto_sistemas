uint64_t main() {
  uint64_t fd;
  uint64_t* p;

  fd = open("mmap_t04.bin", 0, 0);
  if (fd == 4294967295)
    exit(1);

  p = mmap(0, 8192, 0, fd, 0);

  if (*p != 401)
    exit(2);

  // Primera palabra de la segunda página.
  if (*(p + 512) != 402)
    exit(3);

  exit(0);
}
