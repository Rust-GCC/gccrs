// no-system-llvm
// only-x86_64
// build-fail

#![feature(asm)]

// Checks that inline asm errors are mapped to the correct line in the source code.

fn main() {
    unsafe {
        asm!("invalid_instruction");
// { dg-error "" "" { target *-*-* } .-1 }

        asm!("
            invalid_instruction
        ");
// { dg-error "" "" { target *-*-* } .-2 }

        asm!(r#"
            invalid_instruction
        "#);
// { dg-error "" "" { target *-*-* } .-2 }

        asm!("
            mov eax, eax
            invalid_instruction
            mov eax, eax
        ");
// { dg-error "" "" { target *-*-* } .-3 }

        asm!(r#"
            mov eax, eax
            invalid_instruction
            mov eax, eax
        "#);
// { dg-error "" "" { target *-*-* } .-3 }

        asm!(concat!("invalid", "_", "instruction"));
// { dg-error "" "" { target *-*-* } .-1 }

        asm!("movaps %xmm3, (%esi, 2)", options(att_syntax));
// { dg-warning "" "" { target *-*-* } .-1 }

        asm!(
            "invalid_instruction",
        );
// { dg-error "" "" { target *-*-* } .-2 }

        asm!(
            "mov eax, eax",
            "invalid_instruction",
            "mov eax, eax",
        );
// { dg-error "" "" { target *-*-* } .-3 }

        asm!(
            "mov eax, eax\n",
            "invalid_instruction",
            "mov eax, eax",
        );
// { dg-error "" "" { target *-*-* } .-3 }

        asm!(
            "mov eax, eax",
            concat!("invalid", "_", "instruction"),
            "mov eax, eax",
        );
// { dg-error "" "" { target *-*-* } .-3 }

        asm!(
            concat!("mov eax", ", ", "eax"),
            concat!("invalid", "_", "instruction"),
            concat!("mov eax", ", ", "eax"),
        );
// { dg-error "" "" { target *-*-* } .-3 }

        // Make sure template strings get separated
        asm!(
            "invalid_instruction1",
            "invalid_instruction2",
        );
// { dg-error "" "" { target *-*-* } .-3 }
// { dg-error "" "" { target *-*-* } .-3 }

        asm!(
            concat!(
                "invalid", "_", "instruction1", "\n",
                "invalid", "_", "instruction2",
            ),
        );
// { dg-error "" "" { target *-*-* } .-5 }
// { dg-error "" "" { target *-*-* } .-6 }

        asm!(
            concat!(
                "invalid", "_", "instruction1", "\n",
                "invalid", "_", "instruction2",
            ),
            concat!(
                "invalid", "_", "instruction3", "\n",
                "invalid", "_", "instruction4",
            ),
        );
// { dg-error "" "" { target *-*-* } .-9 }
// { dg-error "" "" { target *-*-* } .-10 }
// { dg-error "" "" { target *-*-* } .-7 }
// { dg-error "" "" { target *-*-* } .-8 }

        asm!(
            concat!(
                "invalid", "_", "instruction1", "\n",
                "invalid", "_", "instruction2", "\n",
            ),
            concat!(
                "invalid", "_", "instruction3", "\n",
                "invalid", "_", "instruction4", "\n",
            ),
        );
// { dg-error "" "" { target *-*-* } .-9 }
// { dg-error "" "" { target *-*-* } .-10 }
// { dg-error "" "" { target *-*-* } .-7 }
// { dg-error "" "" { target *-*-* } .-8 }
    }
}

