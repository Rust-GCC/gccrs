// PR libstdc++/124268
// { dg-additional-options "-fmodules -Wno-global-module -Wno-error=expose-global-module-tu-local" }
// { dg-module-cmi M }

module;

static void f() {} // { dg-message "declared here with internal linkage" }
static int x;      // { dg-message "declared here with internal linkage" }

export module M;

namespace ns {
  export using ::f;  // { dg-warning "does not have external linkage" }
  export using ::x;  // { dg-warning "does not have external linkage" }
}
