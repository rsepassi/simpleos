import "os" for Process

var limine = Fn.new { |b, args|
  Process.chdir(b.untar(b.src("limine-8.0.13.tar.gz")))
  b.systemExport([
    "./configure", "--prefix=%(Process.cwd)/build",
    "--enable-uefi-x86-64",
    "--enable-bios",
  ], null, [null, 1, 2])
  b.systemExport(["make", "-j8", "install"], null, [null, 1, 2])
  b.installDir("build/bin")
  b.installDir("build/include")
  b.installDir("build/share")
}
