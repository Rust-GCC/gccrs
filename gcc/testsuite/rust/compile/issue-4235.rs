#[target_feature(enable = "sse")] // { dg-error "the ...target_feature.. attribute may only be applied to functions" }
struct _Test {}
