# simpleos

A minimal OS kernel using the [Limine bootloader](https://github.com/limine-bootloader/limine).

## Build

System dependencies: xos clang lld nasm make mtools parted

```
xos build :boot && ./do/run.sh ./xos-out/xos.img
```

`do/Containerfile` provides an Alpine Linux environment with the necessary
dependencies.

---

Originally forked from [limine-c-template](https://github.com/limine-bootloader/limine-c-template).
