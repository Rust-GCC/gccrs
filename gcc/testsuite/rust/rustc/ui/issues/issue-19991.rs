// Test if the sugared if-let construct correctly prints "missing an else clause" when an else
// clause does not exist, instead of the unsympathetic "`match` arms have incompatible types"

fn main() {
    if let Some(homura) = Some("madoka") { // { dg-error ".E0317." "" { target *-*-* } }
// { dg-error "" "" { target *-*-* } .-2 }
        765
    };
}

