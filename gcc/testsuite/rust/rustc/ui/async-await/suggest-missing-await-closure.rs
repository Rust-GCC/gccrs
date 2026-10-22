// edition:2018
// run-rustfix

#![feature(async_closure)]

fn take_u32(_x: u32) {}

async fn make_u32() -> u32 {
    22
}

#[allow(unused)]
async fn suggest_await_in_async_closure() {
    async || {
        let x = make_u32();
        take_u32(x)
// { dg-error ".E0308." "" { target *-*-* } .-1 }
// { help ".E0308." "" { target *-*-* } .-2 }
// { suggestion ".E0308." "" { target *-*-* } .-3 }
    };
}

fn main() {}

