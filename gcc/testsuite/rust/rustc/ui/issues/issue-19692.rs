struct Homura;

fn akemi(homura: Homura) {
    let Some(ref madoka) = Some(homura.kaname()); // { dg-error ".E0599." "" { target *-*-* } }
    madoka.clone();
}

fn main() { }

