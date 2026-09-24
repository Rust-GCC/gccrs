struct R<'a> {
    r: &'a R<'a>,
}

fn foo(res: Result<u32, &R>) -> u32 {
    let Ok(x) = res;
// { dg-error ".E0005." "" { target *-*-* } .-1 }
    x
}

fn main() {
    foo(Ok(23));
}

