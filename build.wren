import "log" for Log

var REPOS = {
  "xos": {
    "url": "file:///tmp/xos2.tar.gz",
    "hash": "7151201d6cf625686ffd62ac6ba5fb287910ffd2c0aef8b005fdd7e53082fdf8",
  },
}

var cflags = [
  "-std=c11",
  "-fno-stack-protector",
  "-fno-stack-check",
  "-fno-lto",
  "-ffunction-sections",
  "-fdata-sections",
  "-fshort-wchar",
  "-mno-implicit-float",
  "-mno-red-zone",
  "-mno-80387",
  "-mno-mmx",
  "-mno-sse",
  "-mno-sse2",
  "-mcmodel=kernel",
]

var libkernel = Fn.new { |b, args|
  var zig = b.dep("@xos//toolchains/zig")

  var chdrs = b.dep("deps:chdrs")
  var uefi = b.dep("deps:uefi_hdrs")
  var flanterm = b.dep("deps/flanterm")
  var klibc = b.dep("deps/klibc")
  var limineh = b.deptool("deps/limine")

  b.srcGlob("src/*.h")
  zig.exec(b, ["build-lib",
    "-O", zig.getOpt(b.opt_mode),
    "-target", "x86_64-freestanding",
    "--name", "kernel",
    "-cflags"
  ] + cflags + [
    "--",
    "-isystem", chdrs.includeDir,
    "-I", limineh.includeDir,
    "-I", uefi.includeDir,
    "-I", "%(uefi.includeDir)/X64",
    "-I", flanterm.includeDir,
    "-I", klibc.includeDir,
  ] + b.srcGlob("src/*.c"))
  b.installLib("libkernel.a")
}

var kernel = Fn.new { |b, args|
  var zig = b.dep("@xos//toolchains/zig")

  var deps = [
    b.dep(":libkernel").lib("kernel"),
    b.dep("deps/flanterm").lib("flanterm"),
    b.dep("deps/klibc").lib("c"),
  ]

  zig.exec(b, ["cc",
    "-O%(zig.getCCOpt(b.opt_mode))",
    "-target", "x86_64-freestanding",
    "-o", "kernel",
    "-z", "max-page-size=0x1000",
    "-Wl,--gc-sections",
    "-e", "kmain",
    "-T", b.src("src/kernel.ld"),
  ] + deps)

  b.installExe("kernel")
}

var boot = Fn.new { |b, args|
  var kernel = b.dep(":kernel")
  var limine = b.deptool("deps/limine")
  var stdio = Log.level == Log.DEBUG ? [null, 1, 2] : null
  b.systemExport([
    "sh", b.src("do/build_boot.sh"),
    b.src("src/limine.conf"),
    kernel.exe("kernel"),
    limine.path,
  ], null, stdio)
  b.install("", "build/boot/xos.img")
}
