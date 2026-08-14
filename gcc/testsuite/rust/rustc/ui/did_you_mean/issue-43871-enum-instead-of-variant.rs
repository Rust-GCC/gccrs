enum Example { Ex(String), NotEx }

enum Void {}

enum ManyVariants {
    One,
    Two,
    Three,
    Four,
    Five,
    Six,
    Seven,
    Eight,
    Nine,
    Ten,
}

fn result_test() {
    let x = Option(1); // { dg-error ".E0423." "" { target *-*-* } }

    if let Option(_) = x { // { dg-error ".E0532." "" { target *-*-* } }
        println!("It is OK.");
    }

    let y = Example::Ex(String::from("test"));

    if let Example(_) = y { // { dg-error ".E0532." "" { target *-*-* } }
        println!("It is OK.");
    }

    let y = Void(); // { dg-error ".E0423." "" { target *-*-* } }

    let z = ManyVariants(); // { dg-error ".E0423." "" { target *-*-* } }
}

fn main() {}

