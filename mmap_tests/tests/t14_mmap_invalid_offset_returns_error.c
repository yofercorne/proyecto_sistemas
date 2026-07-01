uint64_t main() {
  uint64_t fd;
  uint64_t result;

  fd = open("mmap_t14.bin", 2, 0);
  if (fd == 4294967295)
    exit(1);

  // Offset no múltiplo de 4096. Debe fallar.
  result = mmap(0, 4096, 2, fd, 8);

  if (result != 4294967295)
    exit(2);

  exit(0);
}
