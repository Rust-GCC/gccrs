// check-pass

#![feature(stmt_expr_attributes)]
#![warn(unused_attributes)] // { dg-note "" "" { target *-*-* } }

fn main() {
    // fold_stmt (Item)
    #[allow(dead_code)]
    #[derive(Debug)] // should not warn
    struct Foo;

    // fold_stmt (Mac)
    #[derive(Debug)]
// { dg-warning "" "" { target *-*-* } .-1 }
// { dg-note "" "" { target *-*-* } .-2 }
    println!("Hello, world!");

    // fold_stmt (Semi)
    #[derive(Debug)] // { dg-warning "" "" { target *-*-* } }
    "Hello, world!";

    // fold_stmt (Local)
    #[derive(Debug)] // { dg-warning "" "" { target *-*-* } }
    let _ = "Hello, world!";

    // visit_expr
    let _ = #[derive(Debug)] "Hello, world!";
// { dg-warning "" "" { target *-*-* } .-1 }

    let _ = [
        // filter_map_expr
        #[derive(Debug)] // { dg-warning "" "" { target *-*-* } }
        "Hello, world!"
    ];
}

