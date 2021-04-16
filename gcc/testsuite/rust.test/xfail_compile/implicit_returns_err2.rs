fn test1(x: i32) -> i32 {
    // { dg-error "expected .i32. got .bool." "" { target *-*-*} .-1 }
    return 1;
    // { dg-warning "unreachable expression" "" { target *-*-* } .+1 }
    true
}

fn test2(x: bool) -> bool {
    // { dg-error "expected .bool. got ...." "" { target *-*-*} .-1 }
    return x;
    // { dg-warning "unreachable expression" "" { target *-*-* } .+1 }
    ()
}

fn main() {
    let a = test1(1);
    let a = test2(true);
}
