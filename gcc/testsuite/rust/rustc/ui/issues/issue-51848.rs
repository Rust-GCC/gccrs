// In case of macro expansion, the errors should be matched using the deepest callsite in the
// macro call stack whose span is in the current file

macro_rules! macro_with_error {
    ( ) => {
        println!("{"); // { dg-error "" "" { target *-*-* } }
    };
}

fn foo() {

}

fn main() {
    macro_with_error!();
    //^ In case of a local macro we want the error to be matched in the macro definition, not here

    println!("}"); // { dg-error "" "" { target *-*-* } }
    //^ In case of an external macro we want the error to be matched here
}

