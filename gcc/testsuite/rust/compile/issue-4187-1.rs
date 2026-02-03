macro x(
    $macro_name:ident,
    $macro2_name:ident,
    $type_name:ident,
    $field_name:ident,
    $const_name:ident
) {
    pub struct $type_name {}

    pub const $const_name: $type_name = $type_name {};

    #[x]
    // { dg-error "macro not found" "" { target *-*-* } .-1 }
    macro_rules! $macro_name {
        () => {};
    }

    pub macro $type_name {
                    (Copy $e:expr) => {},
                    () => {;},

                }
}

x!(test_fields, test_fields2, x, field, MY_CONST);

pub fn check_fields_local() {
    test_fields!(check_fields); // { dg-error "Failed to match any rule within macro" }
}
