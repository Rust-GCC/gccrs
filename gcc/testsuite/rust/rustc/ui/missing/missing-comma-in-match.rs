// run-rustfix

fn main() {
    match &Some(3) {
        &None => 1
        &Some(2) => { 3 }
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-note "" "" { target *-*-* } .-2 }
        _ => 2
    };
}

