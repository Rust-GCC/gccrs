// The output should warn when a loop label is not used. However, it
// should also deal with the edge cases where a label is shadowed,
// within nested loops

// check-pass

#![feature(label_break_value)]
#![warn(unused_labels)]

fn main() {
    'unused_while_label: while 0 == 0 {
// { dg-warning "" "" { target *-*-* } .-1 }
    }

    let opt = Some(0);
    'unused_while_let_label: while let Some(_) = opt {
// { dg-warning "" "" { target *-*-* } .-1 }
    }

    'unused_for_label: for _ in 0..10 {
// { dg-warning "" "" { target *-*-* } .-1 }
    }

    'used_loop_label: loop {
        break 'used_loop_label;
    }

    'used_loop_label_outer_1: for _ in 0..10 {
        'used_loop_label_inner_1: for _ in 0..10 {
            break 'used_loop_label_inner_1;
        }
        break 'used_loop_label_outer_1;
    }

    'used_loop_label_outer_2: for _ in 0..10 {
        'unused_loop_label_inner_2: for _ in 0..10 {
// { dg-warning "" "" { target *-*-* } .-1 }
            break 'used_loop_label_outer_2;
        }
    }

    'unused_loop_label_outer_3: for _ in 0..10 {
// { dg-warning "" "" { target *-*-* } .-1 }
        'used_loop_label_inner_3: for _ in 0..10 {
            break 'used_loop_label_inner_3;
        }
    }

    // You should be able to break the same label many times
    'many_used: loop {
        if true {
            break 'many_used;
        } else {
            break 'many_used;
        }
    }

    // Test breaking many times with the same inner label doesn't break the
    // warning on the outer label
    'many_used_shadowed: for _ in 0..10 {
// { dg-warning "" "" { target *-*-* } .-1 }
        'many_used_shadowed: for _ in 0..10 {
// { dg-warning "" "" { target *-*-* } .-1 }
            if 1 % 2 == 0 {
                break 'many_used_shadowed;
            } else {
                break 'many_used_shadowed;
            }
        }
    }

    'unused_loop_label: loop {
// { dg-warning "" "" { target *-*-* } .-1 }
        break;
    }

    // Make sure unused block labels give warnings...
    'unused_block_label: {
// { dg-warning "" "" { target *-*-* } .-1 }
    }

    // ...and that used ones don't:
    'used_block_label: {
        break 'used_block_label;
    }
}

