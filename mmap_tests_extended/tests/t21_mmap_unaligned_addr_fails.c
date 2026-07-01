uint64_t main() {
  uint64_t fd;
  uint64_t result;

  fd = open("mmap_t21.bin", 2, 0);
  if (fd == 4294967295)
    exit(1);

  result = mmap(2147483656, 4096, 2, fd, 0);

  if (result != 4294967295)
    exit(2);

  exit(0);
}
