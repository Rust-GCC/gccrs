fn main() {
    || {
        if false {
            return "test";
        }
        let a = true;
        a // { dg-error ".E0308." "" { target *-*-* } }
    };

    || -> bool {
        if false {
            return "hello" // { dg-error ".E0308." "" { target *-*-* } }
        };
        let b = true;
        b
    };
}

