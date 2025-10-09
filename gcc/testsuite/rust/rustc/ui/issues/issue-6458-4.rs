fn foo(b: bool) -> Result<bool,String> { // { dg-error ".E0308." "" { target *-*-* } }
    Err("bar".to_string());
}

fn main() {
    foo(false);
}

