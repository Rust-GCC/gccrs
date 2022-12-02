/* { dg-output "3\n" } */
extern "C" {
    fn printf(s: *const i8, ...);
}

#[lang = "add"]
pub trait Add<Rhs = Self> {
    type Output;

    fn add(self, rhs: Rhs) -> Self::Output;
}

struct Foo(i32);

impl Add for Foo {
    type Output = Foo;

    fn add(self, other: Foo) -> Foo {
        let res = Foo(self.0 + other.0);

        unsafe {
            let a = "%i\n\0";
            let b = a as *const str;
            let c = b as *const i8;

            printf(c, res.0);
        }

        res
    }
}

fn main() -> i32 {
    let a;
    a = Foo(1) + Foo(2);

    0
}
