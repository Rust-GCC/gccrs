trait DynEq {}

impl<'a> PartialEq for &'a (dyn DynEq + 'static) {
    fn eq(&self, _other: &Self) -> bool {
        true
    }
}

impl Eq for &dyn DynEq {} // { dg-error ".E0308." "" { target *-*-* } }

fn main() {
}

