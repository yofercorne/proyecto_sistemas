# Tests para mmap / munmap / msync en Selfie

## Uso

Desde la carpeta donde está `selfie`:

```bash
unzip mmap_tests.zip
cd mmap_tests
SELFIE=../selfie ./run_mmap_tests.sh
```

Si el binario `selfie` está en la misma carpeta que `mmap_tests`:

```bash
SELFIE=./selfie ./run_mmap_tests.sh
```

También puedes cambiar la memoria:

```bash
MEM=4 SELFIE=../selfie ./run_mmap_tests.sh
```

## Qué cubren los tests

- Lectura básica de una página mapeada.
- Mapping con offset.
- Redondeo de length a PAGESIZE.
- Mapping de dos páginas.
- Dos mappings del mismo archivo compartiendo cache frame.
- Offsets distintos no compartiendo cache frame.
- Persistencia con msync.
- munmap sin msync no persiste a disco.
- Segundo munmap falla.
- msync después de munmap falla.
- fork hereda mappings y comparte frames.
- child hace msync y persiste.
- mmap con dirección explícita alineada.
- mmap inválido por offset.
- mmap inválido por permisos.
- mmap con colisión de dirección.
- mmap con fd inválido.
- Escritura en mapping read-only debe fallar.
- Lectura en mapping write-only debe fallar.
- Acceso después de munmap debe fallar.

## Nota

Los tests t17, t18 y t19 son negativos: deben fallar en runtime. El runner los marca como PASS cuando observa una falla.
