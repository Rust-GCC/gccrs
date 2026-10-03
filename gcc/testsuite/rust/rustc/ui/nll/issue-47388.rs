struct FancyNum {
    num: u8,
}

fn main() {
    let mut fancy = FancyNum{ num: 5 };
    let fancy_ref = &(&mut fancy);
    fancy_ref.num = 6; // { dg-error ".E0594." "" { target *-*-* } }
    println!("{}", fancy_ref.num);
}

