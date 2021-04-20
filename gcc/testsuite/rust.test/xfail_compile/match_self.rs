pub struct Struct;

impl Struct {
    pub fn method(&mut self) {
        match self { // dg-ice
            _ => {},
        }
    }
}

fn main() {
}
