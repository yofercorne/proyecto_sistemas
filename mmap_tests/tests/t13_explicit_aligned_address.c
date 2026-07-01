uint64_t main() {
  uint64_t fd;
  uint64_t* p;

  fd = open("mmap_t13.bin", 0, 0);
  if (fd == 4294967295)
    exit(1);

  // Dirección alineada. Debe funcionar si está libre.
  p = mmap(2147483648, 4096, 0, fd, 0);

  if (*p != 1301)
    exit(2);

  exit(0);
}
