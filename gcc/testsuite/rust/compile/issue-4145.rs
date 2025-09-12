struct S {
    field: [u8; {
        #[path = "outer/inner.rs"]
        mod inner;
        // OK
        0
    }],
}
