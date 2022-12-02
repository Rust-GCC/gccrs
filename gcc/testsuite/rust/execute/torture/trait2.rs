/* { dg-output "Bar::A = 456\n<Foo as Bar>::A = 456\n" } */
extern "C" {
    fn printf(s: *const i8, ...);
}

trait Foo {
    const A: i32 = 123;
}

struct Bar;
impl Foo for Bar {
    const A: i32 = 456;
}

fn main() -> i32 {
    let a;
    a = Bar::A;

    unsafe {
        let _a = "Bar::A = %i\n\0";
        let _b = _a as *const str;
        let _c = _b as *const i8;
        printf(_c, a);
    }

    let b;
    b = <Bar as Foo>::A;

    unsafe {
        let _a = "<Foo as Bar>::A = %i\n\0";
        let _b = _a as *const str;
        let _c = _b as *const i8;
        printf(_c, b);
    }

    0
}
