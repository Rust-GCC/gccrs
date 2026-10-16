#![feature(untagged_unions)]

#[derive(Clone, Copy)]
struct Copy;
struct NonCopy;

union Unn {
    n1: NonCopy,
    n2: NonCopy,
}
union Ucc {
    c1: Copy,
    c2: Copy,
}
union Ucn {
    c: Copy,
    n: NonCopy,
}

fn main() {
    unsafe {
        // 2 NonCopy
        {
            let mut u = Unn { n1: NonCopy };
            let a = u.n1;
            let a = u.n1; // { dg-error ".E0382." "" { target *-*-* } }
        }
        {
            let mut u = Unn { n1: NonCopy };
            let a = u.n1;
            let a = u; // { dg-error ".E0382." "" { target *-*-* } }
        }
        {
            let mut u = Unn { n1: NonCopy };
            let a = u.n1;
            let a = u.n2; // { dg-error ".E0382." "" { target *-*-* } }
        }
        // 2 Copy
        {
            let mut u = Ucc { c1: Copy };
            let a = u.c1;
            let a = u.c1; // OK
        }
        {
            let mut u = Ucc { c1: Copy };
            let a = u.c1;
            let a = u; // OK
        }
        {
            let mut u = Ucc { c1: Copy };
            let a = u.c1;
            let a = u.c2; // OK
        }
        // 1 Copy, 1 NonCopy
        {
            let mut u = Ucn { c: Copy };
            let a = u.c;
            let a = u.c; // OK
        }
        {
            let mut u = Ucn { c: Copy };
            let a = u.n;
            let a = u.n; // { dg-error ".E0382." "" { target *-*-* } }
        }
        {
            let mut u = Ucn { c: Copy };
            let a = u.n;
            let a = u.c; // { dg-error ".E0382." "" { target *-*-* } }
        }
        {
            let mut u = Ucn { c: Copy };
            let a = u.c;
            let a = u.n; // OK
        }
        {
            let mut u = Ucn { c: Copy };
            let a = u.c;
            let a = u; // OK
        }
        {
            let mut u = Ucn { c: Copy };
            let a = u.n;
            let a = u; // { dg-error ".E0382." "" { target *-*-* } }
        }
    }
}

