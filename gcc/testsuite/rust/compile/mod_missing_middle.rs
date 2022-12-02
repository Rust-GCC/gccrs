// { dg-additional-options "-w" }

mod missing_middle {
    mod sub;

    #[path = "explicit.not.rs"]
    mod explicit;
}

#[path = "missing_middle"]
mod with_outer_path_attr {
    #[path = "outer_path.rs"]
    mod inner;
}

mod with_inner_path_attr {
    #![path = "missing_middle"]

    #[path = "inner_path.rs"]
    mod inner;
}

#[path = "missing_middle"]
mod with_both_path_attr {
    #![path = "this_is_ignored"]

    #[path = "both_path.rs"]
    mod inner;
}
