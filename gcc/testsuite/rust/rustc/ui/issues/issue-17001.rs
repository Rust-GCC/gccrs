mod foo {}

fn main() {
    let p = foo { x: () }; // { dg-error ".E0574." "" { target *-*-* } }
}

