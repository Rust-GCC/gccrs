struct A(String);
struct C(D);

fn suggest_remove_deref() {
    let a = &A("".to_string());
    let b = *a;
// { dg-error ".E0507." "" { target *-*-* } .-1 }
}

fn suggest_borrow() {
    let a = [A("".to_string())];
    let b = a[0];
// { dg-error ".E0508." "" { target *-*-* } .-1 }
}

fn suggest_borrow2() {
    let mut a = A("".to_string());
    let r = &&mut a;
    let s = **r;
// { dg-error ".E0507." "" { target *-*-* } .-1 }
}

fn suggest_borrow3() {
    use std::rc::Rc;
    let mut a = A("".to_string());
    let r = Rc::new(a);
    let s = *r;
// { dg-error ".E0507." "" { target *-*-* } .-1 }
}

fn suggest_borrow4() {
    let a = [A("".to_string())][0];
// { dg-error ".E0508." "" { target *-*-* } .-1 }
}

fn suggest_borrow5() {
    let a = &A("".to_string());
    let A(s) = *a;
// { dg-error ".E0507." "" { target *-*-* } .-1 }
}

fn suggest_ref() {
    let c = C(D(String::new()));
    let C(D(s)) = c;
// { dg-error ".E0509." "" { target *-*-* } .-1 }
}

fn suggest_nothing() {
    let a = &A("".to_string());
    let b;
    b = *a;
// { dg-error ".E0507." "" { target *-*-* } .-1 }
}

enum B {
    V(String),
    U(D),
}

struct D(String);

impl Drop for D {
    fn drop(&mut self) {}
}

struct F(String, String);

impl Drop for F {
    fn drop(&mut self) {}
}

fn probably_suggest_borrow() {
    let x = [B::V(String::new())];
    match x[0] {
// { dg-error ".E0508." "" { target *-*-* } .-1 }
        B::U(d) => (),
        B::V(s) => (),
    }
}

fn have_to_suggest_ref() {
    let x = B::V(String::new());
    match x {
// { dg-error ".E0509." "" { target *-*-* } .-1 }
        B::V(s) => drop(s),
        B::U(D(s)) => (),
    };
}

fn two_separate_errors() {
    let x = (D(String::new()), &String::new());
    match x {
// { dg-error ".E0507." "" { target *-*-* } .-1 }
// { dg-error ".E0507." "" { target *-*-* } .-2 }
        (D(s), &t) => (),
        _ => (),
    }
}

fn have_to_suggest_double_ref() {
    let x = F(String::new(), String::new());
    match x {
// { dg-error ".E0509." "" { target *-*-* } .-1 }
        F(s, mut t) => (),
        _ => (),
    }
}

fn double_binding(x: &Result<String, String>) {
    match *x {
// { dg-error ".E0507." "" { target *-*-* } .-1 }
        Ok(s) | Err(s) => (),
    }
}

fn main() {
}

