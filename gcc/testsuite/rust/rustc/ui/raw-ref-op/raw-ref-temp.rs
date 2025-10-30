// Ensure that we don't allow taking the address of temporary values
#![feature(raw_ref_op, type_ascription)]

const FOUR: u64 = 4;

const PAIR: (i32, i64) = (1, 2);

const ARRAY: [i32; 2] = [1, 2];

fn main() {
    let ref_expr = &raw const 2;                        // { dg-error ".E0745." "" { target *-*-* } }
    let mut_ref_expr = &raw mut 3;                      // { dg-error ".E0745." "" { target *-*-* } }
    let ref_const = &raw const FOUR;                    // { dg-error ".E0745." "" { target *-*-* } }
    let mut_ref_const = &raw mut FOUR;                  // { dg-error ".E0745." "" { target *-*-* } }

    let field_ref_expr = &raw const (1, 2).0;           // { dg-error ".E0745." "" { target *-*-* } }
    let mut_field_ref_expr = &raw mut (1, 2).0;         // { dg-error ".E0745." "" { target *-*-* } }
    let field_ref = &raw const PAIR.0;                  // { dg-error ".E0745." "" { target *-*-* } }
    let mut_field_ref = &raw mut PAIR.0;                // { dg-error ".E0745." "" { target *-*-* } }

    let index_ref_expr = &raw const [1, 2][0];          // { dg-error ".E0745." "" { target *-*-* } }
    let mut_index_ref_expr = &raw mut [1, 2][0];        // { dg-error ".E0745." "" { target *-*-* } }
    let index_ref = &raw const ARRAY[0];                // { dg-error ".E0745." "" { target *-*-* } }
    let mut_index_ref = &raw mut ARRAY[1];              // { dg-error ".E0745." "" { target *-*-* } }

    let ref_ascribe = &raw const (2: i32);              // { dg-error ".E0745." "" { target *-*-* } }
    let mut_ref_ascribe = &raw mut (3: i32);            // { dg-error ".E0745." "" { target *-*-* } }

    let ascribe_field_ref = &raw const (PAIR.0: i32);   // { dg-error ".E0745." "" { target *-*-* } }
    let ascribe_index_ref = &raw mut (ARRAY[0]: i32);   // { dg-error ".E0745." "" { target *-*-* } }
}

