struct Z { }

impl Z {
    fn run(&self, z: &mut Z) { }
    fn start(&mut self) {
        self.run(&mut self); // { dg-error ".E0502." "" { target *-*-* } }
// { dg-error "" "" { target *-*-* } .-2 }
    }
}

fn main() {
    let mut z = Z {};
    z.start();
}

