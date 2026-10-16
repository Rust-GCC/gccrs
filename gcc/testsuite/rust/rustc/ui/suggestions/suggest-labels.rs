#[allow(unreachable_code, unused_labels)]
fn main() {
    'foo: loop {
        break 'fo; // { dg-error ".E0426." "" { target *-*-* } }
    }

    'bar: loop {
        continue 'bor; // { dg-error ".E0426." "" { target *-*-* } }
    }

    'longlabel: loop {
        'longlabel1: loop {
            break 'longlable; // { dg-error ".E0426." "" { target *-*-* } }
        }
    }
}

