enum Ast<'a> {
    Num(usize),
    Add(&'a Ast<'a>, &'a Ast<'a>)
}

fn mk_add_bad1<'a,'b>(x: &'a Ast<'a>, y: &'b Ast<'b>) -> Ast<'a> {
    Ast::Add(x, y) // { dg-error ".E0623." "" { target *-*-* } }
}

fn main() {
}

