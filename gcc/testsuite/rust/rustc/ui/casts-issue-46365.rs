struct Lorem {
    ipsum: Ipsum // { dg-error ".E0412." "" { target *-*-* } }
}

fn main() {
    let _foo: *mut Lorem = core::ptr::null_mut(); // no error here
}

