unsafe fn foo<A>() {
    extern "C" {
        static baz: *const A;
// { dg-error ".E0401." "" { target *-*-* } .-1 }
    }

    let bar: *const u64 = core::mem::transmute(&baz);
}

fn main() { }

