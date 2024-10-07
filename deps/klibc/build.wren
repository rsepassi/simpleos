import "os" for Process

var klibc = Fn.new { |b, args|
  Process.chdir(b.untar(b.fetch(
    "https://musl.libc.org/releases/musl-1.2.5.tar.gz",
    "a9a118bbe84d8764da0ea0d28b3ab3fae8477fc7e4085d90102b8596fc7c75e4")))
  var zig = b.deptool("@xos//toolchains/zig")
  zig.ez.cLib(b, {
    "name": "c",
    "flags": [
      "-Iarch/x86_64",
      "-Iarch/generic",
      "-Isrc/include",
      "-Isrc/internal",
      "-I", b.srcDir("include"),
      "-Iinclude",
      "-ffreestanding",
      "-fomit-frame-pointer",
      "-fno-unwind-tables",
      "-fno-asynchronous-unwind-tables",
      "-ffunction-sections",
      "-fdata-sections",
    ],
    "srcs": [
      // Memory
      "src/string/memcpy.c",
      "src/string/memcmp.c",
      "src/string/memset.c",
      "src/string/memmove.c",
      // String
      "src/string/memchr.c",
      "src/string/strrchr.c",
      "src/string/memrchr.c",
      "src/string/strlen.c",
      "src/string/strrchr.c",
      "src/string/strnlen.c",
    ],
  })
}

var printf = Fn.new { |b, args|
  Process.chdir(b.untar(b.fetch(
    "https://api.github.com/repos/mpaland/printf/tarball/d3b9846",
    "3b244e27d7ad5b67aa179e8074455fcfb20b7505c97ba19ffcbee8cf41109176")))
  var zig = b.deptool("@xos//toolchains/zig")
  zig.ez.cLib(b, {
    "srcs": ["printf.c"],
    "include": ["printf.h"],
  })
}
