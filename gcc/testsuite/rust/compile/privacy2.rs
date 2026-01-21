// { dg-additional-options "-w" }

mod orange {
    fn tangerine() {}

    mod green {
        mod blue {
            use super::super::tangerine;
            fn berry() {
                tangerine();
            }
        }
    }
}
