// Check that closures do not implement `Clone` if their environment is not `Clone`.

struct S(i32);

fn main() {
    let a = S(5);
    let hello = move || {
        println!("Hello {}", a.0);
    };

    let hello = hello.clone(); // { dg-error ".E0277." "" { target *-*-* } }
}

