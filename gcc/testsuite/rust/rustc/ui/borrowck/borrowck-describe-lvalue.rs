// ignore-tidy-linelength

pub struct Foo {
  x: u32
}

pub struct Bar(u32);

pub enum Baz {
    X(u32)
}

union U {
    a: u8,
    b: u64,
}

impl Foo {
  fn x(&mut self) -> &mut u32 { &mut self.x }
}

impl Bar {
    fn x(&mut self) -> &mut u32 { &mut self.0 }
}

impl Baz {
    fn x(&mut self) -> &mut u32 {
        match *self {
            Baz::X(ref mut value) => value
        }
    }
}

fn main() {
    // Local and field from struct
    {
        let mut f = Foo { x: 22 };
        let x = f.x();
        f.x; // { dg-error ".E0503." "" { target *-*-* } }
        drop(x);
    }
    // Local and field from tuple-struct
    {
        let mut g = Bar(22);
        let x = g.x();
        g.0; // { dg-error ".E0503." "" { target *-*-* } }
        drop(x);
    }
    // Local and field from tuple
    {
        let mut h = (22, 23);
        let x = &mut h.0;
        h.0; // { dg-error ".E0503." "" { target *-*-* } }
        drop(x);
    }
    // Local and field from enum
    {
        let mut e = Baz::X(2);
        let x = e.x();
        match e {
            Baz::X(value) => value // { dg-error ".E0503." "" { target *-*-* } }
        };
        drop(x);
    }
    // Local and field from union
    unsafe {
        let mut u = U { b: 0 };
        let x = &mut u.a;
        u.a; // { dg-error ".E0503." "" { target *-*-* } }
        drop(x);
    }
    // Deref and field from struct
    {
        let mut f = Box::new(Foo { x: 22 });
        let x = f.x();
        f.x; // { dg-error ".E0503." "" { target *-*-* } }
        drop(x);
    }
    // Deref and field from tuple-struct
    {
        let mut g = Box::new(Bar(22));
        let x = g.x();
        g.0; // { dg-error ".E0503." "" { target *-*-* } }
        drop(x);
    }
    // Deref and field from tuple
    {
        let mut h = Box::new((22, 23));
        let x = &mut h.0;
        h.0; // { dg-error ".E0503." "" { target *-*-* } }
        drop(x);
    }
    // Deref and field from enum
    {
        let mut e = Box::new(Baz::X(3));
        let x = e.x();
        match *e {
            Baz::X(value) => value
// { dg-error ".E0503." "" { target *-*-* } .-1 }
        };
        drop(x);
    }
    // Deref and field from union
    unsafe {
        let mut u = Box::new(U { b: 0 });
        let x = &mut u.a;
        u.a; // { dg-error ".E0503." "" { target *-*-* } }
        drop(x);
    }
    // Constant index
    {
        let mut v = &[1, 2, 3, 4, 5, 6, 7, 8, 9, 10];
        let x = &mut v;
        match v {
            &[x, _, .., _, _] => println!("{}", x),
// { dg-error ".E0503." "" { target *-*-* } .-1 }
                            _ => panic!("other case"),
        }
        match v {
            &[_, x, .., _, _] => println!("{}", x),
// { dg-error ".E0503." "" { target *-*-* } .-1 }
                            _ => panic!("other case"),
        }
        match v {
            &[_, _, .., x, _] => println!("{}", x),
// { dg-error ".E0503." "" { target *-*-* } .-1 }
                            _ => panic!("other case"),
        }
        match v {
            &[_, _, .., _, x] => println!("{}", x),
// { dg-error ".E0503." "" { target *-*-* } .-1 }
                            _ => panic!("other case"),
        }
        drop(x);
    }
    // Subslices
    {
        let mut v = &[1, 2, 3, 4, 5];
        let x = &mut v;
        match v {
            &[x @ ..] => println!("{:?}", x),
// { dg-error ".E0503." "" { target *-*-* } .-1 }
            _ => panic!("other case"),
        }
        match v {
            &[_, x @ ..] => println!("{:?}", x),
// { dg-error ".E0503." "" { target *-*-* } .-1 }
            _ => panic!("other case"),
        }
        match v {
            &[x @ .., _] => println!("{:?}", x),
// { dg-error ".E0503." "" { target *-*-* } .-1 }
            _ => panic!("other case"),
        }
        match v {
            &[_, x @ .., _] => println!("{:?}", x),
// { dg-error ".E0503." "" { target *-*-* } .-1 }
            _ => panic!("other case"),
        }
        drop(x);
    }
    // Downcasted field
    {
        enum E<X> { A(X), B { x: X } }

        let mut e = E::A(3);
        let x = &mut e;
        match e {
            E::A(ref ax) =>
// { dg-error ".E0502." "" { target *-*-* } .-1 }
// { dg-error ".E0502." "" { target *-*-* } .-2 }
                println!("e.ax: {:?}", ax),
            E::B { x: ref bx } =>
// { dg-error ".E0502." "" { target *-*-* } .-1 }
                println!("e.bx: {:?}", bx),
        }
        drop(x);
    }
    // Field in field
    {
        struct F { x: u32, y: u32 };
        struct S { x: F, y: (u32, u32), };
        let mut s = S { x: F { x: 1, y: 2}, y: (999, 998) };
        let x = &mut s;
        match s {
            S  { y: (ref y0, _), .. } =>
// { dg-error ".E0502." "" { target *-*-* } .-1 }
                println!("y0: {:?}", y0),
            _ => panic!("other case"),
        }
        match s {
            S  { x: F { y: ref x0, .. }, .. } =>
// { dg-error ".E0502." "" { target *-*-* } .-1 }
                println!("x0: {:?}", x0),
            _ => panic!("other case"),
        }
        drop(x);
    }
    // Field of ref
    {
        struct Block<'a> {
            current: &'a u8,
            unrelated: &'a u8,
        };

        fn bump<'a>(mut block: &mut Block<'a>) {
            let x = &mut block;
            let p: &'a u8 = &*block.current;
// { dg-error ".E0502." "" { target *-*-* } .-1 }
            // See issue rust#38899
            drop(x);
        }
    }
    // Field of ptr
    {
        struct Block2 {
            current: *const u8,
            unrelated: *const u8,
        }

        unsafe fn bump2(mut block: *mut Block2) {
            let x = &mut block;
            let p : *const u8 = &*(*block).current;
// { dg-error ".E0502." "" { target *-*-* } .-1 }
            // See issue rust#38899
            drop(x);
        }
    }
    // Field of index
    {
        struct F {x: u32, y: u32};
        let mut v = &[F{x: 1, y: 2}, F{x: 3, y: 4}];
        let x = &mut v;
        v[0].y;
// { dg-error ".E0503." "" { target *-*-* } .-1 }
// { dg-error ".E0503." "" { target *-*-* } .-2 }
        drop(x);
    }
    // Field of constant index
    {
        struct F {x: u32, y: u32};
        let mut v = &[F{x: 1, y: 2}, F{x: 3, y: 4}];
        let x = &mut v;
        match v {
            &[_, F {x: ref xf, ..}] => println!("{}", xf),
// { dg-error ".E0502." "" { target *-*-* } .-1 }
            _ => panic!("other case")
        }
        drop(x);
    }
    // Field from upvar
    {
        let mut x = 0;
        || {
            let y = &mut x;
            &mut x; // { dg-error ".E0499." "" { target *-*-* } }
            *y = 1;
        };
    }
    // Field from upvar nested
    {
        let mut x = 0;
           || {
               || { // { dg-error "" "" { target *-*-* } }
                   let y = &mut x;
                   &mut x; // { dg-error ".E0499." "" { target *-*-* } }
                   *y = 1;
                   drop(y);
                }
           };
    }
    {
        fn foo(x: Vec<i32>) {
            let c = || {
                drop(x);
                drop(x); // { dg-error ".E0382." "" { target *-*-* } }
            };
            c();
        }
    }
}

