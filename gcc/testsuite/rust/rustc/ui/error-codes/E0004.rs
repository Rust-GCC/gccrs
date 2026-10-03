enum Terminator {
    HastaLaVistaBaby,
    TalkToMyHand,
}

fn main() {
    let x = Terminator::HastaLaVistaBaby;

    match x { // { dg-error ".E0004." "" { target *-*-* } }
        Terminator::TalkToMyHand => {}
    }
}

