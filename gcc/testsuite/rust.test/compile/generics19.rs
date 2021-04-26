fn foo<'a>(t: &'a str)-> &'a str { // { dg-warning "function is never used: `foo`" }
    // { dg-warning "unused name" "" { target *-*-* } .-1 }
    t
}
