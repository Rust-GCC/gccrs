/* { dg-do compile } */
/* { dg-options "-Wno-return-type" } */

typedef struct a *b;
template <typename c, typename d> c e(d);
enum f { g, h, i };
enum j { k, l, m, aa };
struct a {
  f ab;
};
struct af {
} n;
struct ah;
class al;
b q;
void r(a *);
struct o : af {};
struct p : o {};
struct ag : p {};
j s(af *);
b t();
f fn4();
unsigned fn5(ag *);
b u(ag *, unsigned);
af *v(b(b), b);
static bool w(al *, af *, b(b), b aw, b *ax, j) {
  r(ax[2]);
  if (aw)
    return false;
}
ag *ay;
void x(al *at, b as(b), b bb, b bc) {
  switch (bb->ab)
  case i:
    if (af *bd = v(as, bb))
      if (e<ah *>(bd))
	switch (fn4())
	case h: {
	  b be;
	  switch (be->ab)
	  case i:
	    if (af *bf = v(as, be))
	      if (e<ah *>(bf))
		if (ag *bg = e<ag *>(bf))
		  switch (s(bg)) {
		  case k:
		    if (fn5(bg) == 1) {
		      b bh = u(bg, 0);
		      bh = t();
		      switch (bc->ab)
		      case g: {
			b ax[]{be, bh, bc};
			w(at, &n, as, q, ax, k);
		      }
		    }
		    break;
		  case l:
		    if (fn5(bg) == 1) {
		      b bh = u(bg, 0);
		      bh = t();
		      switch (bc->ab)
		      case g: {
			b ax[]{be, bh, bc};
			w(at, &n, as, q, ax, l);
		      }
		    }
		  }
	}
	  else switch (s(ay)) {
	  case k:
	    if (fn5(ay) == 1) {
	      b be = u(ay, 0);
	      be = t();
	      switch (bc->ab)
	      case g: {
		b ax[]{bb, be, bc};
		w(at, &n, as, q, ax, k);
	      }
	    }
	    break;
	  case l:
	    if (fn5(ay) == 1) {
	      b be = u(ay, 0);
	      be = t();
	      switch (bc->ab)
	      case g: {
		b ax[]{bb, be, bc};
		w(at, &n, as, q, ax, l);
	      }
	    }
	    break;
	  case m:
	    if (fn5(ay) == 1) {
	      b be = u(ay, 0);
	      be = t();
	      switch (bc->ab)
	      case g: {
		b ax[]{bb, be, bc};
		w(at, &n, as, q, ax, m);
	      }
	    }
	    break;
	  case aa:
	    if (fn5(ay) == 1) {
	      b be = u(ay, 0);
	      be = t();
	      switch (bc->ab)
	      case g: {
		b ax[]{bb, be, bc};
		w(at, &n, as, q, ax, aa);
	      }
	    }
	  }
}
