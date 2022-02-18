macro_rules! sep {
    ($($a:expr) => *) => {
        31
    };
    ($($a:expr)  , *) => {
        32
    };
    ($($a:expr)       ;*) => {
        33
    };
    ($($a:literal)custom_sep*) => {
        33
    }
}

fn main() {
    {
        let a0 = sep!(a + b => 15);
        let a1 = sep!(a);
        let a2 = sep!("gcc" => "rust" => "linux");
    }
    {
        let a0 = sep!(a + b, 15);
        let a1 = sep!(a);
        let a2 = sep!("gcc", "rust", "linux");
    }
    {
        let a0 = sep!(a + b;15);
        let a1 = sep!(a);
        let a2 = sep!("gcc";"rust";"linux");
    }
    {
        let a0 = sep!(14 custom_sep 15);
        let a1 = sep!('a');
        let a2 = sep!("gcc" custom_sep "rust" custom_sep "linux");
    }
}
