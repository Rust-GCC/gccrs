struct Example {
    example: Box<dyn Fn(i32) -> i32>
}

fn main() {
    let demo = Example {
        example: Box::new(|x| {
            x + 1
        })
    };

    demo.example(1);
// { dg-error ".E0599." "" { target *-*-* } .-1 }
    // (demo.example)(1);
}

