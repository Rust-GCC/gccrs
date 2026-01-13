// { dg-options "-w" }
#[target_feature(sse)] // { dg-error "attribute can only be applied" }
fn foo() {}