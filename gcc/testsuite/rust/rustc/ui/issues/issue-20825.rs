pub trait Subscriber {
    type Input;
}

pub trait Processor: Subscriber<Input = Self::Input> {
// { dg-error ".E0391." "" { target *-*-* } .-1 }
    type Input;
}

fn main() {}

