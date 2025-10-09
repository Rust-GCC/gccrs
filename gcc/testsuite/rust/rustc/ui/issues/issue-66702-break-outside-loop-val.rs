// Breaks with values inside closures used to ICE (#66863)

fn main() {
    'some_label: loop {
        || break 'some_label ();
// { dg-error ".E0267." "" { target *-*-* } .-1 }
// { dg-error ".E0267." "" { target *-*-* } .-2 }
    }
}

