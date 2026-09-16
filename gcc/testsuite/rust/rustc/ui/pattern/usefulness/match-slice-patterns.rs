fn check(list: &[Option<()>]) {
    match list {
// { dg-error ".E0004." "" { target *-*-* } .-1 }
        &[] => {},
        &[_] => {},
        &[_, _] => {},
        &[_, None, ..] => {},
        &[.., Some(_), _] => {},
    }
}

fn main() {}

