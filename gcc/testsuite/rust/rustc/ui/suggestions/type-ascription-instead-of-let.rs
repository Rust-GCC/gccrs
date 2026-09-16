fn fun(x: i32) -> i32 { x }

fn main() {
    let closure_annotated = |value: i32| -> i32 {
        temp: i32 = fun(5i32);
// { dg-error ".E0425." "" { target *-*-* } .-1 }
        temp + value + 1
// { dg-error ".E0425." "" { target *-*-* } .-1 }
    };
}

