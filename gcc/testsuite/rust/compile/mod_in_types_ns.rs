mod foo {
    mod bar {}

    struct bar; // { dg-error "defined multiple times" }
}
