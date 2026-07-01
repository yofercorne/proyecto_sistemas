uint64_t main() {
  uint64_t fd;
  uint64_t result;
  uint64_t* p;

  fd = open("mmap_t16.bin", 2, 0);
  if (fd == 4294967295)
    exit(1);

  p = mmap(2147483648, 4096, 2, fd, 0);

  if (*p != 1601)
    exit(2);

  // Segunda reserva en la misma dirección debe fallar.
  result = mmap(2147483648, 4096, 2, fd, 0);

  if (result != 4294967295)
    exit(3);

  exit(0);
}
