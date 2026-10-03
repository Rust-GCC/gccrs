// This is not autofixable because we give extra suggestions to end the first expression with `;`.
fn foo(a: Option<u32>, b: Option<u32>) -> bool {
    if let Some(x) = a { true } else { false }
// { dg-error ".E0308." "" { target *-*-* } .-1 }
// { dg-error ".E0308." "" { target *-*-* } .-2 }
    && // { dg-error ".E0308." "" { target *-*-* } }
    if let Some(y) = a { true } else { false }
}

fn main() {}

