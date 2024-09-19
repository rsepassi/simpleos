# simpleos

A minimal OS kernel using the [Limine bootloader](https://github.com/limine-bootloader/limine).

Forked from [limine-c-template](https://github.com/limine-bootloader/limine-c-template).

## Build

```
./mk  # builds limine, kernel, image
./do/run.sh  # runs image in qemu-system-x86_64
```

Dependencies: clang lld nasm make mtools parted

`do/Containerfile` provides an Alpine Linux environment with the necessary
dependencies.
