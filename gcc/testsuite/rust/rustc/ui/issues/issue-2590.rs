struct Parser {
    tokens: Vec<isize> ,
}

trait Parse {
    fn parse(&self) -> Vec<isize> ;
}

impl Parse for Parser {
    fn parse(&self) -> Vec<isize> {
        self.tokens // { dg-error ".E0507." "" { target *-*-* } }
    }
}

fn main() {}

