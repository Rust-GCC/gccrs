enum Homura {
    Akemi { madoka: () }
}

fn main() {
    let homura = Homura::Akemi { kaname: () };
// { dg-error ".E0559." "" { target *-*-* } .-1 }
}

