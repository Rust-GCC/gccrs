mod unclosed_delim_mod;

fn main() {
    let _: usize = unclosed_delim_mod::new();
// { dg-error ".E0308." "" { target *-*-* } .-1 }
}

