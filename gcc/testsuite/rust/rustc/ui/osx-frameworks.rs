// ignore-macos this is supposed to succeed on osx

#[link(name = "foo", kind = "framework")]
extern {}
// { dg-error ".E0455." "" { target *-*-* } .-2 }

fn main() {
}

