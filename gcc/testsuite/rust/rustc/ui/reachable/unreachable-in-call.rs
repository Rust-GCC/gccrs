#![allow(dead_code)]
#![deny(unreachable_code)]

fn diverge() -> ! { panic!() }

fn get_u8() -> u8 {
    1
}
fn call(_: u8, _: u8) {

}
fn diverge_first() {
    call(diverge(),
         get_u8()); // { dg-error "" "" { target *-*-* } }
}
fn diverge_second() {
    call( // { dg-error "" "" { target *-*-* } }
        get_u8(),
        diverge());
}

fn main() {}

