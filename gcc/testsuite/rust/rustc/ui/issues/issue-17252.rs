const FOO: usize = FOO; // { dg-error "" "" { target *-*-* } }

fn main() {
    let _x: [u8; FOO]; // caused stack overflow prior to fix
    let _y: usize = 1 + {
        const BAR: usize = BAR;
        let _z: [u8; BAR]; // caused stack overflow prior to fix
        1
    };
}

