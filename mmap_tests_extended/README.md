# Tests extendidos para mmap / munmap / msync en Selfie

## Uso

Desde la carpeta donde está `selfie`:

```bash
unzip mmap_tests_extended.zip
cd mmap_tests_extended
SELFIE=../selfie ./run_mmap_tests.sh
```

Con más memoria:

```bash
MEM=4 SELFIE=../selfie ./run_mmap_tests.sh
```

## Tests nuevos agregados

- `t21`: `mmap` con dirección no alineada debe fallar.
- `t22`: `mmap` con `length = 0` debe fallar.
- `t23`: `msync` de dos páginas debe persistir offset 0 y offset 4096.
- `t24`: `munmap` de dos páginas y luego `msync` debe fallar.
- `t25`: hijo hace `munmap`, pero el mapping del padre sigue válido.
- `t26`: padre hace `munmap`, pero el mapping del hijo sigue válido.
- `t27`: fork antes de mmap; hijo y padre abren el mismo archivo por separado y deben reutilizar page cache.
- `t28`: dos archivos distintos con offset 0 no deben compartir frame.
- `t29`: archivo menor que una página debe rellenar con cero el resto del cache frame.
- `t30`: `msync` debe escribir correctamente en la segunda página del archivo.

## Tests negativos

`t17`, `t18` y `t19` deben fallar en runtime. El runner los considera PASS cuando observa la falla.
