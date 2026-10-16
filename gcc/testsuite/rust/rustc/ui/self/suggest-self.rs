struct Foo {
    x: i32,
}

impl Foo {
    fn this1(&self) -> i32 {
        let this = self;
        let a = 1;
        this.x
    }

    fn this2(&self) -> i32 {
        let a = Foo {
            x: 2
        };
        let this = a;
        this.x
    }

    fn foo(&self) -> i32 {
        this.x
// { dg-error ".E0425." "" { target *-*-* } .-1 }
    }

    fn bar(&self) -> i32 {
        this.foo()
// { dg-error ".E0425." "" { target *-*-* } .-1 }
    }

    fn baz(&self) -> i32 {
        my.bar()
// { dg-error ".E0425." "" { target *-*-* } .-1 }
    }
}

fn main() {
    let this = vec![1, 2, 3];
    let my = vec![1, 2, 3];
    let len = this.len();
    let len = my.len();
}

