uint64_t main() {
  uint64_t fd;
  uint64_t r;
  uint64_t* p;

  fd = open("mmap_t09.bin", 2, 0);
  if (fd == 4294967295)
    exit(1);

  p = mmap(0, 4096, 2, fd, 0);

  r = munmap(p);
  if (r != 0)
    exit(2);

  r = munmap(p);

  // Selfie syscalls devuelven -1 como 4294967295.
  if (r != 4294967295)
    exit(3);

  exit(0);
}
