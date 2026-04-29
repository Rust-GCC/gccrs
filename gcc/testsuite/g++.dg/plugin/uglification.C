// Test that uglification_plugin.cc works properly.
// { dg-options "-O0 -std=c++20" }

// Pretend this is in a libstdc++-v3 header.
#line 6 "gcc/libstdc++-v3/include/bits/universe.h"

// Stuff outside of namespace std not reported.
int
foo (int a, int b)			// { dg-bogus "note: non-uglified name '\[ab]'" }
{
  int c = a + b;			// { dg-bogus "note: non-uglified name 'c'" }
  return c;
}

namespace std {
  struct foo {
    int bar (int d, int __e) {		// { dg-message "note: non-uglified name 'd'" }
      int f = d + __e;			// { dg-message "note: non-uglified name 'f'" }
      int _E12 = f;			// { dg-message "note: badname '_E12'" }
      using g = int;			// { dg-message "note: non-uglified name 'g'" }
      return f + _E12;
    }
    template <typename T>		// { dg-message "note: non-uglified name 'T'" }
    int baz (int __h, int __tg_foo) {	// { dg-message "note: badname '__tg_foo'" }
      int i = __h;			// { dg-message "note: non-uglified name 'i'" }
      using j = int;			// { dg-message "note: non-uglified name 'j'" }
      return i;
    }
    template <double>
    int baz (int aa) {			// { dg-message "note: non-uglified name 'aa'" }
      int ab = aa;			// { dg-message "note: non-uglified name 'ab'" }
      using ac = double;		// { dg-message "note: non-uglified name 'ac'" }
      return ab;
    }
    int zz;				// { dg-bogus "note: non-uglified name 'zz'" }
  };
  template <typename _T>		// { dg-message "note: badname '_T'" }
  struct qux {
    int corge (int k, int __l) {	// { dg-message "note: non-uglified name 'k'" }
      int __maskrune = k + __l;		// { dg-message "note: badname '__maskrune'" }
      typedef std::foo __in_opt;	// { dg-message "note: badname '__in_opt'" }
      return __maskrune;
    }
    template <typename _Tp>
    int garply (int m) {		// { dg-message "note: non-uglified name 'm'" }
      int n = m;			// { dg-message "note: non-uglified name 'n'" }
      struct foo { int baz; } __o;	// { dg-message "note: non-uglified whitelisted name 'foo'" }
      __o.baz = n;
      return __o.baz;
    }
  };
  template <typename _Tp, typename _Up>
  struct S {
    S () = delete;
    ~S () = delete;
  };
  template <typename _Tp>
  struct S <_Tp, _Tp> {
    int grault (int ad) {		// { dg-message "note: non-uglified name 'ad'" }
      int ae = ad;			// { dg-message "note: non-uglified name 'ae'" }
      return ae;
    }
    int quux () { return 0; }
  };
  template <>
  struct S <int, long> {
    int grault (int af) {		// { dg-message "note: non-uglified name 'af'" }
      int ag = af;			// { dg-message "note: non-uglified name 'ag'" }
      return ag;
    }
  };

  inline namespace __cxx11 {
    enum freddy {
      frog, waldo
    };
    struct fred {
      int garply (int p) {		// { dg-message "note: non-uglified name 'p'" }
	int _res_ext = p;		// { dg-message "note: non-uglified name '_res_ext'" }
	return _res_ext;
      }
    };
  }
  namespace __gnu_cxx {
    template <typename ...U>		// { dg-message "note: non-uglified name 'U'" }
    int foo (U... q) {			// { dg-message "note: non-uglified name 'q'" }
      return 0;				// { dg-message "note: non-uglified whitelisted name 'foo'" "" { target *-*-* } .-1 }
    }
    enum blah {				// { dg-message "note: non-uglified name 'blah'" }
      plugh,				// { dg-message "note: non-uglified name 'plugh'" }
      thud,				// { dg-message "note: non-uglified name 'thud'" }
      _E5,				// { dg-message "note: badname '_E5'" }
      garply,				// { dg-message "note: non-uglified whitelisted name 'garply'" }
      grault,				// { dg-message "note: non-uglified whitelisted name 'grault'" }
      quux,				// { dg-message "note: non-uglified whitelisted name 'quux'" }
      lorem,				// { dg-message "note: non-uglified whitelisted name 'lorem'" }
      ipsum,				// { dg-message "note: non-uglified whitelisted name 'ipsum'" }
      dolor,				// { dg-message "note: non-uglified whitelisted name 'dolor'" }
      sit,				// { dg-message "note: non-uglified whitelisted name 'sit'" }
      amet,				// { dg-message "note: non-uglified whitelisted name 'amet'" }
      consectetur,			// { dg-message "note: non-uglified whitelisted name 'consectetur'" }
      adipiscing,			// { dg-message "note: non-uglified whitelisted name 'adipiscing'" }
      elit				// { dg-message "note: non-uglified whitelisted name 'elit'" }
    };
    int waldo (int r) {			// { dg-message "note: non-uglified name 'r'" }
      using s = std::foo;		// { dg-message "note: non-uglified name 's'" }
      return r;				// { dg-message "note: non-uglified whitelisted name 'waldo'" "" { target *-*-* } .-2 }
    }
    int xyzzy () { return 0; }		// { dg-message "note: non-uglified whitelisted name 'xyzzy'" }
    int barf () { return 0; }		// { dg-message "note: non-uglified name 'barf'" }
    int __foobar () { return 0; }
    struct V {				// { dg-message "note: non-uglified name 'V'" }
      union {
	int t;				// { dg-message "note: non-uglified name 't'" }
      };
      int zz () { return 0; }		// { dg-message "note: non-uglified whitelisted name 'zz'" }
      int ww () { return 0; }		// { dg-message "note: non-uglified name 'ww'" }
    };
  }
  int xyzzy (int s) {			// { dg-message "note: non-uglified name 's'" }
    return s;
  }
  template <int _Idx>
  struct lorem {
    template <int _Idx2>
    friend int ipsum (int ah) {		// { dg-message "note: non-uglified name 'ah'" }
      int ai = ah;			// { dg-message "note: non-uglified name 'ai'" }
      return ai;
    }
  };
  template <int _Idx>
  struct dolor {
    friend int sit (int aj) {		// { dg-message "note: non-uglified name 'aj'" }
      int ak = aj;			// { dg-message "note: non-uglified name 'ak'" }
      return ak;
    }
  };
  struct amet {
    template <int _Idx>
    friend int consectetur (int al) {	// { dg-message "note: non-uglified name 'al'" }
      int am = al;			// { dg-message "note: non-uglified name 'am'" }
      return am;
    }
  };
  struct adipiscing {
    friend int elit (int an) {		// { dg-message "note: non-uglified name 'an'" }
      int ao = an;			// { dg-message "note: non-uglified name 'ao'" }
      return ao;
    }
  };
}
