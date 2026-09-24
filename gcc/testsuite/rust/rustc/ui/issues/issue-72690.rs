fn no_err() {
    |x: String| x;
    let _ = String::from("x");
}

fn err() {
    String::from("x".as_ref()); // { dg-error ".E0283." "" { target *-*-* } }
}

fn arg_pat_closure_err() {
    |x| String::from("x".as_ref()); // { dg-error ".E0283." "" { target *-*-* } }
}

fn local_pat_closure_err() {
    let _ = "x".as_ref(); // { dg-error ".E0283." "" { target *-*-* } }
}

fn err_first_arg_pat() {
    String::from("x".as_ref()); // { dg-error ".E0283." "" { target *-*-* } }
    |x: String| x;
}

fn err_second_arg_pat() {
    |x: String| x;
    String::from("x".as_ref()); // { dg-error ".E0283." "" { target *-*-* } }
}

fn err_mid_arg_pat() {
    |x: String| x;
    |x: String| x;
    |x: String| x;
    |x: String| x;
    String::from("x".as_ref()); // { dg-error ".E0283." "" { target *-*-* } }
    |x: String| x;
    |x: String| x;
    |x: String| x;
    |x: String| x;
}

fn err_first_local_pat() {
    String::from("x".as_ref()); // { dg-error ".E0283." "" { target *-*-* } }
    let _ = String::from("x");
}

fn err_second_local_pat() {
    let _ = String::from("x");
    String::from("x".as_ref()); // { dg-error ".E0283." "" { target *-*-* } }
}

fn err_mid_local_pat() {
    let _ = String::from("x");
    let _ = String::from("x");
    let _ = String::from("x");
    let _ = String::from("x");
    String::from("x".as_ref()); // { dg-error ".E0283." "" { target *-*-* } }
    let _ = String::from("x");
    let _ = String::from("x");
    let _ = String::from("x");
    let _ = String::from("x");
}

fn main() {}

