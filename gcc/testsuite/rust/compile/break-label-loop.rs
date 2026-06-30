// `break 'label loop {}` used to ICE in the code generator because the
// labeled loop never registered its result temporary against the label.
#![feature(no_core)]
#![no_core]

pub fn foo() {
    'a: loop {
        break 'a loop {};
    }
}
