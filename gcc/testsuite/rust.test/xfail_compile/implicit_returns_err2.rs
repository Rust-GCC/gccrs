fn test(x: i32) -> i32 {
    return true; // { dg-error "expected .i32. got .bool." }
    1 // { dg-warning "unreachable expression" }
}

fn main() {
    let a = test(1);
}
