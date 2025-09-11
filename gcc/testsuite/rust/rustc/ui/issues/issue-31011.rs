macro_rules! log {
    ( $ctx:expr, $( $args:expr),* ) => {
        if $ctx.trace {
// { dg-error ".E0609." "" { target *-*-* } .-1 }
            println!( $( $args, )* );
        }
    }
}

// Create a structure.
struct Foo {
  trace: bool,
}

// Generic wrapper calls log! with a structure.
fn wrap<T>(context: &T) -> ()
{
    log!(context, "entered wrapper");
// { dg-error "" "" { target *-*-* } .-1 }
}

fn main() {
    // Create a structure.
    let x = Foo { trace: true };
    log!(x, "run started");
    // Apply a closure which accesses internal fields.
    wrap(&x);
    log!(x, "run finished");
}

