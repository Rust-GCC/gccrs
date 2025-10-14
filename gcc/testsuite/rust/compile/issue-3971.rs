#[lang = "copy"]
trait Copy {}

#[derive(Copy)]
// { dg-error "derive may only be applied to structs, enums and unions" "" { target *-*-* } .-1 }

pub fn check_ge(a: i32, b: i32) -> bool {
    a >= b
}

