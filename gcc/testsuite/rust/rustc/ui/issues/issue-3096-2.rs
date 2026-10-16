enum Bottom { }

fn main() {
    let x = &() as *const () as *const Bottom;
    match x { } // { dg-error ".E0004." "" { target *-*-* } }
}

