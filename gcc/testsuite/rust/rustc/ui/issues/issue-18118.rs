pub fn main() {
    const z: &'static isize = {
        let p = 3;
        &p // { dg-error ".E0597." "" { target *-*-* } }
    };
}

