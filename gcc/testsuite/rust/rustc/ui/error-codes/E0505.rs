struct Value {}

fn eat(val: Value) {}

fn main() {
    let x = Value{};
    {
        let _ref_to_val: &Value = &x;
        eat(x); // { dg-error ".E0505." "" { target *-*-* } }
        _ref_to_val.use_ref();
    }
}

trait Fake { fn use_mut(&mut self) { } fn use_ref(&self) { }  }
impl<T> Fake for T { }

