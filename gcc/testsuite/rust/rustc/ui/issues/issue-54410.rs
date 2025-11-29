extern "C" {
    pub static mut symbol: [i8];
// { dg-error ".E0277." "" { target *-*-* } .-1 }
}

fn main() {
    println!("{:p}", unsafe { &symbol });
}

