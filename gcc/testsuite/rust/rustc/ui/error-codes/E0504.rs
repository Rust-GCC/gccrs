struct FancyNum {
    num: u8,
}

fn main() {
    let fancy_num = FancyNum { num: 5 };
    let fancy_ref = &fancy_num;

    let x = move || { // { dg-error ".E0505." "" { target *-*-* } }
        println!("child function: {}", fancy_num.num);
    };

    x();
    println!("main function: {}", fancy_ref.num);
}

