uint64_t main() {
  uint64_t result;

  // fd inventado. Debe fallar porque no hay file_id registrado.
  result = mmap(0, 4096, 2, 12345, 0);

  if (result != 4294967295)
    exit(1);

  exit(0);
}
