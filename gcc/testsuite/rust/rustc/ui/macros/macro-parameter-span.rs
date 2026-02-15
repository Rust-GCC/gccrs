macro_rules! foo {
    ($id: ident) => {
        $id
    }
}

// Testing that the error span points to the parameter 'x' in the callsite,
// not to the macro variable '$id'
fn main() {
    foo!(
        x // { dg-error ".E0425." "" { target *-*-* } }
        );
}

