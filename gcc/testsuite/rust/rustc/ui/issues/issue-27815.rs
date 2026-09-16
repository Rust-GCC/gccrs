mod A {}

fn main() {
    let u = A { x: 1 }; // { dg-error ".E0574." "" { target *-*-* } }
    let v = u32 { x: 1 }; // { dg-error ".E0574." "" { target *-*-* } }
    match () {
        A { x: 1 } => {}
// { dg-error ".E0574." "" { target *-*-* } .-1 }
        u32 { x: 1 } => {}
// { dg-error ".E0574." "" { target *-*-* } .-1 }
    }
}

