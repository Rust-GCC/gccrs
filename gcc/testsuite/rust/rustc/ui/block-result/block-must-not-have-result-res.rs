struct R;

impl Drop for R {
    fn drop(&mut self) {
        true // { dg-error ".E0308." "" { target *-*-* } }
    }
}

fn main() {
}

