struct S {f:String}
impl Drop for S {
    fn drop(&mut self) { println!("{}", self.f); }
}

fn move_in_match() {
    match (S {f:"foo".to_string()}) {
// { dg-error ".E0509." "" { target *-*-* } .-1 }
        S {f:_s} => {}
    }
}

fn move_in_let() {
    let S {f:_s} = S {f:"foo".to_string()};
// { dg-error ".E0509." "" { target *-*-* } .-1 }
}

fn move_in_fn_arg(S {f:_s}: S) {
// { dg-error ".E0509." "" { target *-*-* } .-1 }
}

fn main() {}

