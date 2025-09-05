#[allow(unreachable_code)]

fn main() {
    loop {
        break while continue { // { dg-error ".E0590." "" { target *-*-* } }
        }
    }
}

