// check that borrowck looks inside consts/statics

static FN : &'static (dyn Fn() -> (Box<dyn Fn()->Box<i32>>) + Sync) = &|| {
    let x = Box::new(0);
    Box::new(|| x) // { dg-error ".E0507." "" { target *-*-* } }
};

fn main() {
    let f = (FN)();
    f();
    f();
}

