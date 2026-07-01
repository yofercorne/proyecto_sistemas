uint64_t main() {
  uint64_t fd;
  uint64_t x;
  uint64_t* p;

  fd = open("mmap_t19.bin", 2, 0);
  if (fd == 4294967295)
    exit(1);

  p = mmap(0, 4096, 2, fd, 0);

  if (munmap(p) != 0)
    exit(2);

  // Esta lectura debería fallar porque el mapping ya no existe.
  x = *p;

  if (x == 0)
    exit(0);

  // Si llega aquí, hay bug.
  exit(0);
}
