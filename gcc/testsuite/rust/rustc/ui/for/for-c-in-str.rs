// E0277 should point exclusively at line 6, not the entire for loop span

fn main() {
    for c in "asdf" {
// { dg-error ".E0277." "" { target *-*-* } .-1 }
// { dg-note ".E0277." "" { target *-*-* } .-2 }
// { help ".E0277." "" { target *-*-* } .-3 }
// { dg-note ".E0277." "" { target *-*-* } .-4 }
// { dg-note ".E0277." "" { target *-*-* } .-5 }
// { dg-note ".E0277." "" { target *-*-* } .-6 }
// { dg-note ".E0277." "" { target *-*-* } .-7 }
// { dg-note ".E0277." "" { target *-*-* } .-8 }
// { dg-note ".E0277." "" { target *-*-* } .-9 }
        println!();
    }
}

