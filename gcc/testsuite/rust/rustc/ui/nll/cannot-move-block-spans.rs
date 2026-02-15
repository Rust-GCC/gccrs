// Test that the we point to the inner expression when moving out to initialize
// a variable, and that we don't give a useless suggestion such as &{ *r }.

pub fn deref(r: &String) {
    let x = { *r }; // { dg-error ".E0507." "" { target *-*-* } }
    let y = unsafe { *r }; // { dg-error ".E0507." "" { target *-*-* } }
    let z = loop { break *r; }; // { dg-error ".E0507." "" { target *-*-* } }
}

pub fn index(arr: [String; 2]) {
    let x = { arr[0] }; // { dg-error ".E0508." "" { target *-*-* } }
    let y = unsafe { arr[0] }; // { dg-error ".E0508." "" { target *-*-* } }
    let z = loop { break arr[0]; }; // { dg-error ".E0508." "" { target *-*-* } }
}

pub fn additional_statement_cases(r: &String) {
    let x = { let mut u = 0; u += 1; *r }; // { dg-error ".E0507." "" { target *-*-* } }
    let y = unsafe { let mut u = 0; u += 1; *r }; // { dg-error ".E0507." "" { target *-*-* } }
    let z = loop { let mut u = 0; u += 1; break *r; u += 2; }; // { dg-error ".E0507." "" { target *-*-* } }
}

fn main() {}

