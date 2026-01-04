// { dg-options "-w" }
#[target_feature(enable = "sse2")]
fn foo() {} // { dg-error "attribute can only be applied to .unsafe. functions" }
