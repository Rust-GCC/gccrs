struct S(String);
impl Drop for S {
    fn drop(&mut self) { }
}

fn move_in_match() {
    match S("foo".to_string()) {
// { dg-error ".E0509." "" { target *-*-* } .-1 }
        S(_s) => {}
    }
}

fn move_in_let() {
    let S(_s) = S("foo".to_string());
// { dg-error ".E0509." "" { target *-*-* } .-1 }
}

fn move_in_fn_arg(S(_s): S) {
// { dg-error ".E0509." "" { target *-*-* } .-1 }
}

fn main() {}

