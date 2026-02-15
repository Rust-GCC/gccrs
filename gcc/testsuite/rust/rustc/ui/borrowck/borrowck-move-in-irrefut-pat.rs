fn with<F>(f: F) where F: FnOnce(&String) {}

fn arg_item(&_x: &String) {}
// { dg-error ".E0507." "" { target *-*-* } .-1 }

fn arg_closure() {
    with(|&_x| ())
// { dg-error ".E0507." "" { target *-*-* } .-1 }
}

fn let_pat() {
    let &_x = &"hi".to_string();
// { dg-error ".E0507." "" { target *-*-* } .-1 }
}

pub fn main() {}

