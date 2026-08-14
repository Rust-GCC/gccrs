struct FuncContainer {
    f1: fn(data: u8),
    f2: extern "C" fn(data: u8),
    f3: unsafe fn(data: u8),
}

struct FuncContainerOuter {
    container: Box<FuncContainer>
}

struct Obj<F> where F: FnOnce() -> u32 {
    closure: F,
    not_closure: usize,
}

struct BoxedObj {
    boxed_closure: Box<dyn FnOnce() -> u32>,
}

struct Wrapper<F> where F: FnMut() -> u32 {
    wrap: Obj<F>,
}

fn func() -> u32 {
    0
}

fn check_expression() -> Obj<Box<dyn FnOnce() -> u32>> {
    Obj { closure: Box::new(|| 42_u32) as Box<dyn FnOnce() -> u32>, not_closure: 42 }
}

fn main() {
    // test variations of function

    let o_closure = Obj { closure: || 42, not_closure: 42 };
    o_closure.closure(); // { dg-error ".E0599." "" { target *-*-* } }

    o_closure.not_closure();
// { dg-error ".E0599." "" { target *-*-* } .-1 }

    let o_func = Obj { closure: func, not_closure: 5 };
    o_func.closure(); // { dg-error ".E0599." "" { target *-*-* } }

    let boxed_fn = BoxedObj { boxed_closure: Box::new(func) };
    boxed_fn.boxed_closure();// { dg-error ".E0599." "" { target *-*-* } }

    let boxed_closure = BoxedObj { boxed_closure: Box::new(|| 42_u32) as Box<dyn FnOnce() -> u32> };
    boxed_closure.boxed_closure();// { dg-error ".E0599." "" { target *-*-* } }

    // test expression writing in the notes

    let w = Wrapper { wrap: o_func };
    w.wrap.closure();// { dg-error ".E0599." "" { target *-*-* } }

    w.wrap.not_closure();
// { dg-error ".E0599." "" { target *-*-* } .-1 }

    check_expression().closure();// { dg-error ".E0599." "" { target *-*-* } }
}

impl FuncContainerOuter {
    fn run(&self) {
        unsafe {
            (*self.container).f1(1); // { dg-error ".E0599." "" { target *-*-* } }
            (*self.container).f2(1); // { dg-error ".E0599." "" { target *-*-* } }
            (*self.container).f3(1); // { dg-error ".E0599." "" { target *-*-* } }
        }
    }
}

