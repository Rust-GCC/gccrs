fn array() -> [(String, String); 3] {
    Default::default()
}

// Const Index + Const Index

fn move_out_from_begin_and_end() {
    let a = array();
    match a {
        [_, _, _x] => {}
    }
    match a {
        [.., ref _y] => {} // { dg-error ".E0382." "" { target *-*-* } }
    }
}

fn move_out_from_begin_field_and_end() {
    let a = array();
    match a {
        [_, _, (_x, _)] => {}
    }
    match a {
        [.., ref _y] => {} // { dg-error ".E0382." "" { target *-*-* } }
    }
}

fn move_out_from_begin_field_and_end_field() {
    let a = array();
    match a {
        [_, _, (_x, _)] => {}
    }
    match a {
        [.., (ref _y, _)] => {} // { dg-error ".E0382." "" { target *-*-* } }
    }
}

// Const Index + Slice

fn move_out_by_const_index_and_subslice() {
    let a = array();
    match a {
        [_x, _, _] => {}
    }
    match a {
// { dg-error ".E0382." "" { target *-*-* } .-1 }
        [ref _y @ .., _, _] => {}
    }
}

fn move_out_by_const_index_end_and_subslice() {
    let a = array();
    match a {
        [.., _x] => {}
    }
    match a {
// { dg-error ".E0382." "" { target *-*-* } .-1 }
        [_, _, ref _y @ ..] => {}
    }
}

fn move_out_by_const_index_field_and_subslice() {
    let a = array();
    match a {
        [(_x, _), _, _] => {}
    }
    match a {
// { dg-error ".E0382." "" { target *-*-* } .-1 }
        [ref _y @ .., _, _] => {}
    }
}

fn move_out_by_const_index_end_field_and_subslice() {
    let a = array();
    match a {
        [.., (_x, _)] => {}
    }
    match a {
// { dg-error ".E0382." "" { target *-*-* } .-1 }
        [_, _, ref _y @ ..] => {}
    }
}

fn move_out_by_subslice_and_const_index_field() {
    let a = array();
    match a {
        [_y @ .., _, _] => {}
    }
    match a {
        [(ref _x, _), _, _] => {} // { dg-error ".E0382." "" { target *-*-* } }
    }
}

fn move_out_by_subslice_and_const_index_end_field() {
    let a = array();
    match a {
        [_, _, _y @ ..] => {}
    }
    match a {
        [.., (ref _x, _)] => {} // { dg-error ".E0382." "" { target *-*-* } }
    }
}

// Slice + Slice

fn move_out_by_subslice_and_subslice() {
    let a = array();
    match a {
        [x @ .., _] => {}
    }
    match a {
// { dg-error ".E0382." "" { target *-*-* } .-1 }
        [_, ref _y @ ..] => {}
    }
}

// Move + Assign

fn move_out_and_assign_end() {
    let mut a = array();
    match a {
        [_, _, _x] => {}
    }
    a[2] = Default::default(); // { dg-error ".E0382." "" { target *-*-* } }
}

fn move_out_and_assign_end_field() {
    let mut a = array();
    match a {
        [_, _, (_x, _)] => {}
    }
    a[2].1 = Default::default(); // { dg-error ".E0382." "" { target *-*-* } }
}

fn move_out_slice_and_assign_end() {
    let mut a = array();
    match a {
        [_, _, _x @ ..] => {}
    }
    a[0] = Default::default(); // { dg-error ".E0382." "" { target *-*-* } }
}

fn move_out_slice_and_assign_end_field() {
    let mut a = array();
    match a {
        [_, _, _x @ ..] => {}
    }
    a[0].1 = Default::default(); // { dg-error ".E0382." "" { target *-*-* } }
}

fn main() {}

