// { dg-error "malformed `target_feature` attribute input" }
#[target_feature]
unsafe fn foo_sse() {}
