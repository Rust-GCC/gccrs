pub fn main() {
    const z: &'static isize = {
        static p: isize = 3;
        &p // { dg-error ".E0013." "" { target *-*-* } }
    };
}

