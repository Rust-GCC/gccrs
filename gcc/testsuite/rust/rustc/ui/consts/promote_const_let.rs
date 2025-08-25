fn main() {
    let x: &'static u32 = {
        let y = 42;
        &y // { dg-error ".E0597." "" { target *-*-* } }
    };
    let x: &'static u32 = &{ // { dg-error ".E0716." "" { target *-*-* } }
        let y = 42;
        y
    };
}

