// Test that we don't show variables with from async fn desugaring

// edition:2018

async fn async_fn(&ref mut s: &[i32]) {}
// { dg-error ".E0596." "" { target *-*-* } .-1 }

fn main() {}

