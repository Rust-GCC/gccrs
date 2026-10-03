#![crate_type="lib"]
#![feature(c_variadic)]

pub unsafe extern "C" fn use_vararg_lifetime(
    x: usize,
    y: ...
) -> &usize { // { dg-error ".E0106." "" { target *-*-* } }
    &0
}

pub unsafe extern "C" fn use_normal_arg_lifetime(x: &usize, y: ...) -> &usize { // OK
    x
}

