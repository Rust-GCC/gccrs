enum Question {
    Yes,
    No,
}

mod foo {
    const FOO: u32 = 0;
}

fn main() {
    let x = "a";
    x += 2;
// { dg-error ".E0368." "" { target *-*-* } .-1 }
    y = 2;
// { dg-error ".E0425." "" { target *-*-* } .-1 }
    x.z();
// { dg-error ".E0599." "" { target *-*-* } .-1 }

    !Question::Yes;
// { dg-error ".E0600." "" { target *-*-* } .-1 }

    foo::FOO;
// { dg-error ".E0603." "" { target *-*-* } .-1 }

    0u32 as char;
// { dg-error ".E0604." "" { target *-*-* } .-1 }

    let x = 0u8;
    x as Vec<u8>;
// { dg-error ".E0605." "" { target *-*-* } .-1 }

    let x = 5;
    let x_is_nonzero = x as bool;
// { dg-error ".E0054." "" { target *-*-* } .-1 }

    let x = &0u8;
    let y: u32 = x as u32;
// { dg-error ".E0606." "" { target *-*-* } .-1 }

    let v = core::ptr::null::<u8>();
    v as *const [u8];
// { dg-error ".E0607." "" { target *-*-* } .-1 }
}

