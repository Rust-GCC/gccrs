fn main() {
    enum Color {
        Rgb(usize, usize, usize),
        Cmyk(usize, usize, usize, usize),
        NoColor,
    }

    fn foo(c: Color) {
        match c {
          Color::Rgb(_, _, _) => { }
          Color::Cmyk(_, _, _, _) => { }
          Color::NoColor(_) => { }
// { dg-error ".E0532." "" { target *-*-* } .-1 }
        }
    }
}

