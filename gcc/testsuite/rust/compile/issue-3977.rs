// Test for issue #3977: ICE in fold_convert_loc with diverging expressions in while conditions
// { dg-excess-errors "expected boolean expression" }

fn test_continue() {
    loop {
        while continue {} // { dg-error "expected boolean expression in 'while' condition" }
    }
}

fn test_break() {
    loop {
        while break {} // { dg-error "expected boolean expression in 'while' condition" }
    }
}

fn test_return() {
    while return {} // { dg-error "expected boolean expression in 'while' condition" }
}

fn test_return_with_value() {
    while return 42 {} // { dg-error "expected boolean expression in 'while' condition" }
}

fn test_infinite_loop() {
    while loop {} {} // { dg-error "expected boolean expression in 'while' condition" }
}

fn test_nested_continue() {
    loop {
        loop {
            while continue {} // { dg-error "expected boolean expression in 'while' condition" }
        }
    }
}

fn test_labeled_break() {
    'outer: loop {
        while break 'outer {} // { dg-error "expected boolean expression in 'while' condition" }
    }
}

fn test_labeled_continue() {
    'outer: loop {
        while continue 'outer {} // { dg-error "expected boolean expression in 'while' condition" }
    }
}

// Valid cases that should NOT error
fn test_valid_boolean() {
    while true {}
    while false {}
}

fn test_valid_expression() {
    let x = 5;
    while x > 0 {}
}

fn test_valid_function_call() -> bool {
    fn condition() -> bool { true }
    while condition() {}
    true
}

fn main() {
    test_valid_boolean();
    test_valid_expression();
    test_valid_function_call();
    
    // The error cases would cause compilation to fail
    // so they're tested separately
}