#![feature(thread_local)]

#[thread_local]
static FOO: u8 = 3;

fn main() {
    let a = &FOO;
// { dg-error ".E0712." "" { target *-*-* } .-1 }
// { dg-note ".E0712." "" { target *-*-* } .-2 }

    std::thread::spawn(move || {
        println!("{}", a);
    });
}
// { dg-note "" "" { target *-*-* } .-1 }

