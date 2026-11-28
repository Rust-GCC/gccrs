enum Token { LeftParen, RightParen, Plus, Minus, /* etc */ }
struct Struct {
    a: usize,
}

fn use_token(token: &Token) { unimplemented!() }

fn main() {
    use_token(&Token::Homura); // { dg-error ".E0599." "" { target *-*-* } }
    Struct::method(); // { dg-error ".E0599." "" { target *-*-* } }
    Struct::method; // { dg-error ".E0599." "" { target *-*-* } }
    Struct::Assoc; // { dg-error ".E0599." "" { target *-*-* } }
}

