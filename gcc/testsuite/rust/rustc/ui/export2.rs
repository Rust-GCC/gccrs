mod foo {
    pub fn x() { bar::x(); } // { dg-error ".E0433." "" { target *-*-* } }
}

mod bar {
    fn x() { println!("x"); }

    pub fn y() { }
}

fn main() { foo::x(); }

