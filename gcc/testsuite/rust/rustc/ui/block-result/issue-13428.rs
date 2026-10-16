// Regression test for #13428

fn foo() -> String {  // { dg-error ".E0308." "" { target *-*-* } }
    format!("Hello {}",
            "world")
    // Put the trailing semicolon on its own line to test that the
    // note message gets the offending semicolon exactly
    ;
}

fn bar() -> String {  // { dg-error ".E0308." "" { target *-*-* } }
    "foobar".to_string()
    ;
}

pub fn main() {}

