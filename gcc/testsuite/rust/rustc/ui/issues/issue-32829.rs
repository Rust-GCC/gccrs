static S : u64 = { { panic!("foo"); 0 } };
// { dg-error ".E0658." "" { target *-*-* } .-1 }

fn main() {
    println!("{:?}", S);
}

