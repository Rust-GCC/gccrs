#[link(name = "...", wasm_import_module)] // { dg-error "" "" { target *-*-* } }
extern {}

#[link(name = "...", wasm_import_module(x))] // { dg-error "" "" { target *-*-* } }
extern {}

#[link(name = "...", wasm_import_module())] // { dg-error "" "" { target *-*-* } }
extern {}

fn main() {}

