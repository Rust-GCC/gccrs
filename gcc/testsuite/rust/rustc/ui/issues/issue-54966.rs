// issue-54966: ICE returning an unknown type with impl FnMut

fn generate_duration() -> Oper<impl FnMut()> {}
// { dg-error ".E0412." "" { target *-*-* } .-1 }

fn main() {}

