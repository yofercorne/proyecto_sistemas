#!/usr/bin/env bash
set -u

SELFIE=${SELFIE:-./selfie}
MEM=${MEM:-2}
TEST_DIR=${TEST_DIR:-tests}

make_fixtures() {
python3 - <<'PY'
import struct
from pathlib import Path

def make_file(name, pairs, pages=2):
    size = pages * 4096
    data = bytearray(size)
    for index, value in pairs.items():
        off = index * 8
        data[off:off+8] = struct.pack("<Q", value)
    Path(name).write_bytes(data)

make_file("mmap_t01.bin", {0: 101, 1: 102})
make_file("mmap_t02.bin", {0: 201, 512: 202})
make_file("mmap_t03.bin", {0: 301, 511: 399})
make_file("mmap_t04.bin", {0: 401, 512: 402})
make_file("mmap_t05.bin", {0: 501})
make_file("mmap_t06.bin", {0: 601, 512: 602})
make_file("mmap_t07.bin", {0: 701})
make_file("mmap_t08.bin", {0: 801})
make_file("mmap_t09.bin", {0: 901})
make_file("mmap_t10.bin", {0: 1001})
make_file("mmap_t11.bin", {0: 1101})
make_file("mmap_t12.bin", {0: 1201})
make_file("mmap_t13.bin", {0: 1301})
make_file("mmap_t14.bin", {0: 1401})
make_file("mmap_t15.bin", {0: 1501})
make_file("mmap_t16.bin", {0: 1601})
make_file("mmap_t17.bin", {0: 1701})
make_file("mmap_t18.bin", {0: 1801})
make_file("mmap_t19.bin", {0: 1901})
PY
}

extract_exit_code() {
  local logfile="$1"

  # Intenta extraer exit code desde salidas típicas de Selfie.
  # Si no encuentra nada, el runner usará el código de salida del proceso host.
  grep -Eio 'exit code [0-9]+' "$logfile" | tail -n 1 | awk '{print $3}'
}

run_expect_zero() {
  local test_file="$1"
  local name
  local logfile
  local host_rc
  local sim_rc

  name=$(basename "$test_file" .c)
  logfile=".${name}.log"

  timeout 25 "$SELFIE" -c "$test_file" -m "$MEM" > "$logfile" 2>&1
  host_rc=$?

  sim_rc=$(extract_exit_code "$logfile")
  if [ -z "$sim_rc" ]; then
    sim_rc=$host_rc
  fi

  if [ "$sim_rc" = "0" ] && [ "$host_rc" = "0" ]; then
    echo "PASS  $name"
  else
    echo "FAIL  $name  host_rc=$host_rc sim_rc=$sim_rc"
    tail -n 8 "$logfile"
  fi
}

run_expect_failure() {
  local test_file="$1"
  local name
  local logfile
  local host_rc
  local sim_rc

  name=$(basename "$test_file" .c)
  logfile=".${name}.log"

  timeout 25 "$SELFIE" -c "$test_file" -m "$MEM" > "$logfile" 2>&1
  host_rc=$?

  sim_rc=$(extract_exit_code "$logfile")
  if [ -z "$sim_rc" ]; then
    sim_rc=$host_rc
  fi

  if [ "$sim_rc" != "0" ] || [ "$host_rc" != "0" ]; then
    echo "PASS  $name  expected failure observed"
  else
    echo "FAIL  $name  expected failure, but program exited 0"
    tail -n 8 "$logfile"
  fi
}

check_word() {
python3 - "$1" "$2" <<'PY'
import struct, sys
path = sys.argv[1]
expected = int(sys.argv[2])
with open(path, "rb") as f:
    got = struct.unpack("<Q", f.read(8))[0]
if got != expected:
    print(f"DISK-CHECK-FAIL {path}: expected {expected}, got {got}")
    sys.exit(1)
print(f"DISK-CHECK-PASS {path}: {got}")
PY
}

make_fixtures

echo "== mmap success and semantic tests =="
run_expect_zero "$TEST_DIR/t01_mmap_read_one_page.c"
run_expect_zero "$TEST_DIR/t02_mmap_offset_second_page.c"
run_expect_zero "$TEST_DIR/t03_mmap_length_rounding.c"
run_expect_zero "$TEST_DIR/t04_mmap_two_pages.c"
run_expect_zero "$TEST_DIR/t05_same_file_two_mappings_share.c"
run_expect_zero "$TEST_DIR/t06_different_offsets_do_not_share.c"
run_expect_zero "$TEST_DIR/t07_msync_persists_to_file.c"
check_word mmap_t07.bin 777
run_expect_zero "$TEST_DIR/t08_munmap_without_msync_does_not_persist.c"
check_word mmap_t08.bin 801
run_expect_zero "$TEST_DIR/t09_munmap_twice_fails_second_time.c"
run_expect_zero "$TEST_DIR/t10_msync_after_munmap_fails.c"
run_expect_zero "$TEST_DIR/t11_fork_inherits_mapping_and_shares_frame.c"
run_expect_zero "$TEST_DIR/t12_fork_child_msync_persists.c"
check_word mmap_t12.bin 1222
run_expect_zero "$TEST_DIR/t13_explicit_aligned_address.c"
run_expect_zero "$TEST_DIR/t14_mmap_invalid_offset_returns_error.c"
run_expect_zero "$TEST_DIR/t15_mmap_invalid_prot_returns_error.c"
run_expect_zero "$TEST_DIR/t16_mmap_address_collision_fails.c"
run_expect_zero "$TEST_DIR/t20_mmap_invalid_fd_returns_error.c"

echo
echo "== negative permission/lifetime tests =="
echo "Estos deben fallar en runtime. Si salen como FAIL, tu validación de permisos o munmap está mal."
run_expect_failure "$TEST_DIR/t17_read_only_write_should_fail.c"
run_expect_failure "$TEST_DIR/t18_write_only_read_should_fail.c"
run_expect_failure "$TEST_DIR/t19_access_after_munmap_should_fail.c"
