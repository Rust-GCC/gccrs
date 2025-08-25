// Regression test for issue #27592.

fn write<'a, F: ::std::ops::FnOnce()->::std::fmt::Arguments<'a> + 'a>(fcn: F) {
    use std::fmt::Write;
    let _ = match fcn() { a => write!(&mut Stream, "{}", a), };
}

struct Stream;
impl ::std::fmt::Write for Stream {
    fn write_str(&mut self, _s: &str) -> ::std::fmt::Result {
        Ok( () )
    }
}

fn main() {
    write(|| format_args!("{}", String::from("Hello world")));
// { dg-error ".E0515." "" { target *-*-* } .-1 }
// { dg-error ".E0515." "" { target *-*-* } .-2 }
}

