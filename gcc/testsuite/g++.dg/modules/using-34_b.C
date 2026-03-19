// PR libstdc++/124268
// { dg-additional-options "-fmodules -Wno-global-module -Wno-global-module-tu-local" }
// { dg-module-cmi !K }

module;

// Non vars/functions cannot escape, even when relaxed.
namespace { struct Internal; };  // { dg-message "declared here with internal linkage" }
struct {} none;			 // { dg-message "declared here with no linkage" }
using NoneType = decltype(none);

export module K;
import M;

export using ::Internal;  // { dg-error "does not have external linkage" }
export using ::NoneType;  // { dg-error "does not have external linkage" }

// OK
int test() {
  ns::f();
  return ns::x;
}
