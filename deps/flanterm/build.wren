
var flanterm = Fn.new { |b, args|
  var zig = b.dep("@xos//toolchains/zig")
  zig.ez.cLib(b, {
    "srcs": [b.src("flanterm.c"), b.src("backends/fb.c")],
    "include": [b.src("flanterm.h")],
  })
  b.install("include/flanterm_backends", b.src("backends/fb.h"))
}

