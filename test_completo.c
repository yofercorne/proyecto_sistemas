// Que valida:
// 1) open + registro de fd en fd_file_table
// 2) validaciones de mmap: fd invalido, length cero, offset no alineado,
//    prot invalido y addr no alineado
// 3) mmap con addr = 0 y length no multiplo de pagina, validando round_up
// 4) page cache compartida por (file_id, offset)
// 5) offset distinto evita falso HIT
// 6) region ocupada no puede mapearse otra vez en el mismo addr
// 7) fork comparte mappings y frames de page cache
// 8) msync persiste cambios hacia el archivo
// 9) munmap elimina mappings y errores posteriores son detectados
// 10) remap final confirma persistencia

uint64_t PAGESIZE = 4096;
uint64_t WORDS_PER_PAGE = 512;
uint64_t TWO_PAGES = 8192;
uint64_t TOTAL_WORDS = 1024;

uint64_t PROT_READ = 0;
uint64_t PROT_WRITE = 1;
uint64_t PROT_READ_WRITE = 2;

uint64_t MAP_FAILED = 4294967295;
uint64_t O_CREAT_TRUNC_WRONLY_LINUX = 577;
uint64_t O_RDWR_LINUX = 2;
uint64_t PERM_0644 = 420;

void say(uint64_t* s, uint64_t n) {
  write(1, s, n);
}

void line() {
  say((uint64_t*) "\n", 1);
}

void step(uint64_t* s, uint64_t n) {
  say((uint64_t*) "\n== ", 4);
  say(s, n);
  say((uint64_t*) " ==\n", 4);
}

void ok(uint64_t* s, uint64_t n) {
  say((uint64_t*) "[OK] ", 5);
  say(s, n);
  say((uint64_t*) "\n", 1);
}

void fail(uint64_t code, uint64_t* s, uint64_t n) {
  say((uint64_t*) "[FAIL] ", 7);
  say(s, n);
  say((uint64_t*) "\n", 1);
  exit(code);
}

uint64_t main() {
  uint64_t seed_fd;
  uint64_t fd;
  uint64_t i;
  uint64_t written;
  uint64_t pid;
  uint64_t waited;

  uint64_t* seed;
  uint64_t* status;

  uint64_t* bad;
  uint64_t* a;
  uint64_t* b;
  uint64_t* c;
  uint64_t* d;
  uint64_t* overlap;

  step((uint64_t*) "Preparar archivo de prueba", 26);

  seed = malloc(TWO_PAGES);

  i = 0;
  while (i < TOTAL_WORDS) {
    *(seed + i) = 0;
    i = i + 1;
  }

  *seed = 111;
  *(seed + WORDS_PER_PAGE) = 222;

  seed_fd = open("mmap_demo_test.bin", O_CREAT_TRUNC_WRONLY_LINUX, PERM_0644);
  if (seed_fd == MAP_FAILED)
    fail(1, (uint64_t*) "no se pudo crear archivo", 24);

  written = write(seed_fd, seed, TWO_PAGES);
  if (written != TWO_PAGES)
    fail(2, (uint64_t*) "write inicial no escribio 2 paginas", 35);

  fd = open("mmap_demo_test.bin", O_RDWR_LINUX, PERM_0644);
  if (fd == MAP_FAILED)
    fail(3, (uint64_t*) "no se pudo abrir O_RDWR", 23);

  ok((uint64_t*) "archivo creado y fd registrado", 30);

  step((uint64_t*) "Validaciones negativas de mmap", 30);

  bad = mmap(0, PAGESIZE, PROT_READ, 999999, 0);
  if (bad != MAP_FAILED)
    fail(10, (uint64_t*) "fd invalido no fallo", 20);
  ok((uint64_t*) "fd invalido detectado", 21);

  bad = mmap(0, 0, PROT_READ, fd, 0);
  if (bad != MAP_FAILED)
    fail(11, (uint64_t*) "length cero no fallo", 20);
  ok((uint64_t*) "length cero detectado", 21);

  bad = mmap(0, PAGESIZE, PROT_READ, fd, 1);
  if (bad != MAP_FAILED)
    fail(12, (uint64_t*) "offset no alineado no fallo", 27);
  ok((uint64_t*) "offset no alineado detectado", 28);

  bad = mmap(0, PAGESIZE, 99, fd, 0);
  if (bad != MAP_FAILED)
    fail(13, (uint64_t*) "prot invalido no fallo", 22);
  ok((uint64_t*) "prot invalido detectado", 23);

  bad = mmap(123, PAGESIZE, PROT_READ, fd, 0);
  if (bad != MAP_FAILED)
    fail(14, (uint64_t*) "addr no alineado no fallo", 25);
  ok((uint64_t*) "addr no alineado detectado", 26);

  step((uint64_t*) "mmap principal: addr=0 y round_up", 33);

  a = mmap(0, 5000, PROT_READ_WRITE, fd, 0);
  if (a == MAP_FAILED)
    fail(20, (uint64_t*) "mmap principal fallo", 20);

  if (*a != 111)
    fail(21, (uint64_t*) "pagina 0 no cargo valor inicial", 31);

  if (*(a + WORDS_PER_PAGE) != 222)
    fail(22, (uint64_t*) "round_up no permitio segunda pagina", 35);

  ok((uint64_t*) "mmap de 5000 bytes cubre 2 paginas", 34);

  step((uint64_t*) "Page cache compartida por mismo offset", 38);

  b = mmap(0, PAGESIZE, PROT_READ_WRITE, fd, 0);
  if (b == MAP_FAILED)
    fail(30, (uint64_t*) "segundo mmap mismo offset fallo", 31);

  *a = 333;

  if (*b != 333)
    fail(31, (uint64_t*) "b no vio escritura hecha por a", 30);

  ok((uint64_t*) "a y b comparten frame offset 0", 30);

  step((uint64_t*) "Offset distinto evita falso HIT", 31);

  c = mmap(0, PAGESIZE, PROT_READ_WRITE, fd, PAGESIZE);
  if (c == MAP_FAILED)
    fail(40, (uint64_t*) "mmap offset 4096 fallo", 22);

  if (*c != 222)
    fail(41, (uint64_t*) "offset 4096 no cargo pagina 1", 29);

  *c = 444;

  if (*(a + WORDS_PER_PAGE) != 444)
    fail(42, (uint64_t*) "pagina 1 no comparte frame", 26);

  ok((uint64_t*) "offset 4096 usa su propio frame compartido", 42);

  step((uint64_t*) "Region ocupada", 14);

  overlap = mmap(a, PAGESIZE, PROT_READ, fd, 0);
  if (overlap != MAP_FAILED)
    fail(50, (uint64_t*) "se permitio mapear encima", 25);

  ok((uint64_t*) "is_region_free_for_mmap bloqueo solape", 38);

  step((uint64_t*) "fork con mappings compartidos", 29);

  status = malloc(8);
  *status = 999;

  pid = fork();

  if (pid == 0) {
    ok((uint64_t*) "hijo escribe sobre mapping heredado", 35);

    *a = 777;
    *(a + WORDS_PER_PAGE) = 888;

    if (msync(a) != 0)
      exit(60);

    exit(0);
  }

  if (pid == MAP_FAILED)
    fail(61, (uint64_t*) "fork fallo", 10);

  waited = wait(status);
  if (waited == MAP_FAILED)
    fail(62, (uint64_t*) "wait fallo", 10);

  if (*status != 0)
    fail(63, (uint64_t*) "hijo termino con error", 22);

  if (*a != 777)
    fail(64, (uint64_t*) "padre no vio cambio del hijo pag0", 33);

  if (*(a + WORDS_PER_PAGE) != 888)
    fail(65, (uint64_t*) "padre no vio cambio del hijo pag1", 33);

  ok((uint64_t*) "fork mantuvo mappings compartidos", 33);

  step((uint64_t*) "msync y munmap", 14);

  if (msync(a) != 0)
    fail(70, (uint64_t*) "msync principal fallo", 21);
  ok((uint64_t*) "msync escribio frames al archivo", 32);

  if (munmap(b) != 0)
    fail(71, (uint64_t*) "munmap b fallo", 14);
  ok((uint64_t*) "munmap elimina mapping b", 24);

  if (munmap(b) != MAP_FAILED)
    fail(72, (uint64_t*) "munmap repetido no fallo", 24);
  ok((uint64_t*) "munmap repetido detectado", 25);

  if (msync(b) != MAP_FAILED)
    fail(73, (uint64_t*) "msync sobre b unmapped no fallo", 31);
  ok((uint64_t*) "msync sobre unmapped detectado", 30);

  if (munmap(c) != 0)
    fail(74, (uint64_t*) "munmap c fallo", 14);

  if (munmap(a) != 0)
    fail(75, (uint64_t*) "munmap a fallo", 14);

  ok((uint64_t*) "mappings principales liberados", 30);

  step((uint64_t*) "Remap final: persistencia", 25);

  d = mmap(0, 5000, PROT_READ, fd, 0);
  if (d == MAP_FAILED)
    fail(80, (uint64_t*) "remap final fallo", 17);

  if (*d != 777)
    fail(81, (uint64_t*) "persistencia pag0 fallo", 23);

  if (*(d + WORDS_PER_PAGE) != 888)
    fail(82, (uint64_t*) "persistencia pag1 fallo", 23);

  ok((uint64_t*) "remap confirmo datos persistidos", 32);

  if (munmap(d) != 0)
    fail(83, (uint64_t*) "munmap final fallo", 18);

  step((uint64_t*) "RESULTADO", 9);
  ok((uint64_t*) "de mmap paso correctamente", 40);

  exit(0);
}
