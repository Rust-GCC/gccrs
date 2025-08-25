// edition:2018

fn take_u32(_x: u32) {}

async fn make_u32() -> u32 {
    22
}

#[allow(unused)]
async fn suggest_await_in_async_fn() {
    let x = make_u32();
    take_u32(x)
// { dg-error ".E0308." "" { target *-*-* } .-1 }
// { help ".E0308." "" { target *-*-* } .-2 }
// { suggestion ".E0308." "" { target *-*-* } .-3 }
}

async fn dummy() {}

#[allow(unused)]
async fn suggest_await_in_async_fn_return() {
    dummy()
// { dg-error ".E0308." "" { target *-*-* } .-1 }
// { help ".E0308." "" { target *-*-* } .-2 }
// { help ".E0308." "" { target *-*-* } .-3 }
// { suggestion ".E0308." "" { target *-*-* } .-4 }
}

fn main() {}

