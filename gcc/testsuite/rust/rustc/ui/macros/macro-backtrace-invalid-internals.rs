// Macros in statement vs expression position handle backtraces differently.

macro_rules! fake_method_stmt {
     () => {
          1.fake() // { dg-error ".E0599." "" { target *-*-* } }
     }
}

macro_rules! fake_field_stmt {
     () => {
          1.fake // { dg-error ".E0610." "" { target *-*-* } }
     }
}

macro_rules! fake_anon_field_stmt {
     () => {
          (1).0 // { dg-error ".E0610." "" { target *-*-* } }
     }
}

macro_rules! fake_method_expr {
     () => {
          1.fake() // { dg-error ".E0599." "" { target *-*-* } }
     }
}

macro_rules! fake_field_expr {
     () => {
          1.fake // { dg-error ".E0610." "" { target *-*-* } }
     }
}

macro_rules! fake_anon_field_expr {
     () => {
          (1).0 // { dg-error ".E0610." "" { target *-*-* } }
     }
}

macro_rules! real_method_stmt {
     () => {
          2.0.neg() // { dg-error ".E0689." "" { target *-*-* } }
     }
}

macro_rules! real_method_expr {
     () => {
          2.0.neg() // { dg-error ".E0689." "" { target *-*-* } }
     }
}

fn main() {
    fake_method_stmt!();
    fake_field_stmt!();
    fake_anon_field_stmt!();
    real_method_stmt!();

    let _ = fake_method_expr!();
    let _ = fake_field_expr!();
    let _ = fake_anon_field_expr!();
    let _ = real_method_expr!();
}

