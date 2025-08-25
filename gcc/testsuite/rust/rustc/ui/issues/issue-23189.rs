mod module {}

fn main() {
    let _ = module { x: 0 }; // { dg-error ".E0574." "" { target *-*-* } }
}

