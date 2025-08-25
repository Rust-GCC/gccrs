use zoo::fly; // { dg-error ".E0603." "" { target *-*-* } }

mod zoo {
    fn fly() {}
}


fn main() {
    fly();
}

