struct Struct;

impl Struct {
    fn method(&mut self) -> i32 {
        2718
    }
}

fn main() {
    let a: Struct;
    let value = a.method();
}
