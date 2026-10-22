fn main() {
    enum Color {
        Rgb(usize, usize, usize),
        Cmyk(usize, usize, usize, usize),
        NoColor,
    }

    fn foo(c: Color) {
        match c {
          Color::Rgb(_, _) => { }
// { dg-error ".E0023." "" { target *-*-* } .-1 }
          Color::Cmyk(_, _, _, _) => { }
          Color::NoColor => { }
        }
    }
}

