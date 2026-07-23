fn main() {
    let x = vec![1];
    let y = x;
    x; // { dg-error ".E0382." "" { target *-*-* } }

    let x = vec![1];
    let mut y = x;
    x; // { dg-error ".E0382." "" { target *-*-* } }

    let x = (Some(vec![1]), ());

    match x {
        (Some(y), ()) => {},
        _ => {},
    }
    x; // { dg-error ".E0382." "" { target *-*-* } }
}

