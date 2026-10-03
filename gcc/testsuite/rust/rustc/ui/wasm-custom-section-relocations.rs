// only-wasm32

#[link_section = "test"]
pub static A: &[u8] = &[1]; // { dg-error "" "" { target *-*-* } }

#[link_section = "test"]
pub static B: [u8; 3] = [1, 2, 3];

#[link_section = "test"]
pub static C: usize = 3;

#[link_section = "test"]
pub static D: &usize = &C; // { dg-error "" "" { target *-*-* } }

fn main() {}

