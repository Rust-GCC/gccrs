// ignore-pretty pretty-printing is unhygienic

#![feature(decl_macro)]

mod foo {
    struct S { x: u32 }
    struct T(u32);

    pub macro m($S:ident, $x:ident) {{
        struct $S {
            $x: u32,
            x: i32,
        }

        let s = S { x: 0 }; // { dg-error "" "" { target *-*-* } }
        let _ = s.x; // { dg-error "" "" { target *-*-* } }

        let t = T(0); // { dg-error "" "" { target *-*-* } }
        let _ = t.0; // { dg-error "" "" { target *-*-* } }

        let s = $S { $x: 0, x: 1 };
        assert_eq!((s.$x, s.x), (0, 1));
        s
    }}
}

fn main() {
    let s = foo::m!(S, x);
    assert_eq!(s.x, 0);
}

