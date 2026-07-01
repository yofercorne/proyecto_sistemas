uint64_t main() {
  uint64_t fd;
  uint64_t x;
  uint64_t* p;

  fd = open("mmap_t18.bin", 2, 0);
  if (fd == 4294967295)
    exit(1);

  p = mmap(0, 4096, 1, fd, 0);

  // Este load debería fallar por permisos.
  x = *p;

  // Evitar warning conceptual de variable no usada.
  if (x == 0)
    exit(0);

  // Si llega aquí, hay bug de permisos.
  exit(0);
}
