// Test that closures cannot subvert aliasing restrictions

fn main() {
    // Unboxed closure case
    {
        let mut x = 0;
        let mut f = || &mut x; // { dg-error "" "" { target *-*-* } }
        let x = f();
        let y = f();
    }
}

