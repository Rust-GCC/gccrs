fn test_if() {
    r#if true { } // { dg-error "" "" { target *-*-* } }
}

fn test_struct() {
    r#struct Test; // { dg-error "" "" { target *-*-* } }
}

fn test_union() {
    r#union Test; // { dg-error "" "" { target *-*-* } }
}

fn test_if_2() {
    let _ = r#if; // { dg-error ".E0425." "" { target *-*-* } }
}

fn test_struct_2() {
    let _ = r#struct; // { dg-error ".E0425." "" { target *-*-* } }
}

fn test_union_2() {
    let _ = r#union; // { dg-error ".E0425." "" { target *-*-* } }
}

fn main() {}

