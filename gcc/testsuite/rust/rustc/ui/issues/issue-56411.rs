macro_rules! import {
    ( $(($path:expr, $name:ident)),* ) => {
        $(
            #[path = $path]
            mod $name;
            pub use self::$name;
// { dg-error ".E0365." "" { target *-*-* } .-1 }
// { dg-error ".E0365." "" { target *-*-* } .-2 }

        )*
    }
}

import!(("issue-56411-aux.rs", issue_56411_aux));

fn main() {
    println!("Hello, world!");
}

