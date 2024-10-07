import "os" for Process

// Libfdt
// https://mirrors.edge.kernel.org/pub/software/utils/dtc/dtc-1.7.1.tar.gz

// Musl libc parts
// https://musl.libc.org/releases/musl-1.2.5.tar.gz

var chdrs = Fn.new { |b, args|
  var hdrs = b.untar(b.src("freestnd-c-hdrs-0bsd.tar.gz"))
  b.installHeader(b.glob("%(hdrs)/*.h"))
}

var uefi_hdrs = Fn.new { |b, args|
  Process.chdir(b.untar(b.fetch(
    "https://github.com/tianocore/edk2/archive/refs/tags/edk2-stable202408.tar.gz",
    "63c99b6f9f7aa94e8d76c432bea05d0d4dd6600af78d6fd59a1aec5ce9cea8ce")))
  b.installDir("include", "MdePkg/Include/Uefi")
  b.installDir("include", "MdePkg/Include/X64")
  b.installDir("include", "MdePkg/Include/Guid")
  b.installDir("include", "MdePkg/Include/Protocol")
  b.installDir("include", "MdePkg/Include/IndustryStandard")
  b.installHeader([
    "MdePkg/Include/Uefi.h",
    "MdePkg/Include/Base.h",
  ])
}
