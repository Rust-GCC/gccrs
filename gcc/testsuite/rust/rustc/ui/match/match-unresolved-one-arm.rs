fn foo<T>() -> T { panic!("Rocks for my pillow") }

fn main() {
    let x = match () { // { dg-error ".E0282." "" { target *-*-* } }
        () => foo() // T here should be unresolved
    };
}

