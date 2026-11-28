pub struct SendPacket<T> {
    p: T
}

mod pingpong {
    use SendPacket;
    pub type Ping = SendPacket<Pong>;
    pub struct Pong(SendPacket<Ping>);
// { dg-error ".E0072." "" { target *-*-* } .-1 }
}

fn main() {}

