#[cfg(FALSE)]
fn if_else_parse_error() {
    if true {
    } #[attr] else if false { // { dg-error "" "" { target *-*-* } }
    }
}

#[cfg(FALSE)]
fn else_attr_ifparse_error() {
    if true {
    } else #[attr] if false { // { dg-error "" "" { target *-*-* } }
    } else {
    }
}

#[cfg(FALSE)]
fn else_parse_error() {
    if true {
    } else if false {
    } #[attr] else { // { dg-error "" "" { target *-*-* } }
    }
}

fn main() {
}

