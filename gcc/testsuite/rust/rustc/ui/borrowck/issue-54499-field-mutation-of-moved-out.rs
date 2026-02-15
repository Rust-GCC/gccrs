#![warn(unused)]
#[derive(Debug)]
struct S(i32);

type Tuple = (S, i32);
struct Tpair(S, i32);
struct Spair { x: S, y: i32 }

fn main() {
    {
        let t: Tuple = (S(0), 0);
        drop(t);
        t.0 = S(1);
// { dg-error ".E0382." "" { target *-*-* } .-1 }
// { dg-error ".E0382." "" { target *-*-* } .-2 }
        t.1 = 2;
// { dg-error ".E0594." "" { target *-*-* } .-1 }
        println!("{:?} {:?}", t.0, t.1);
    }

    {
        let u: Tpair = Tpair(S(0), 0);
        drop(u);
        u.0 = S(1);
// { dg-error ".E0382." "" { target *-*-* } .-1 }
// { dg-error ".E0382." "" { target *-*-* } .-2 }
        u.1 = 2;
// { dg-error ".E0594." "" { target *-*-* } .-1 }
        println!("{:?} {:?}", u.0, u.1);
    }

    {
        let v: Spair = Spair { x: S(0), y: 0 };
        drop(v);
        v.x = S(1);
// { dg-error ".E0382." "" { target *-*-* } .-1 }
// { dg-error ".E0382." "" { target *-*-* } .-2 }
        v.y = 2;
// { dg-error ".E0594." "" { target *-*-* } .-1 }
        println!("{:?} {:?}", v.x, v.y);
    }
}

