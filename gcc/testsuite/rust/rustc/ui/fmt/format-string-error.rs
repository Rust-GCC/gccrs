// ignore-tidy-tab

fn main() {
    println!("{");
// { dg-error "" "" { target *-*-* } .-1 }
    println!("{{}}");
    println!("}");
// { dg-error "" "" { target *-*-* } .-1 }
    let _ = format!("{_}", _ = 6usize);
// { dg-error "" "" { target *-*-* } .-1 }
    let _ = format!("{a:_}", a = "", _ = 0);
// { dg-error "" "" { target *-*-* } .-1 }
    let _ = format!("{a:._$}", a = "", _ = 0);
// { dg-error "" "" { target *-*-* } .-1 }
    let _ = format!("{");
// { dg-error "" "" { target *-*-* } .-1 }
    let _ = format!("}");
// { dg-error "" "" { target *-*-* } .-1 }
    let _ = format!("{\\}");
// { dg-error "" "" { target *-*-* } .-1 }
    let _ = format!("\n\n\n{\n\n\n");
// { dg-error "" "" { target *-*-* } .-1 }
    let _ = format!(r###"



	{"###);
// { dg-error "" "" { target *-*-* } .-1 }
    let _ = format!(r###"



	{

"###);
// { dg-error "" "" { target *-*-* } .-1 }
    let _ = format!(r###"



	}

"###);
// { dg-error "" "" { target *-*-* } .-3 }
    let _ = format!(r###"



        }

"###);
// { dg-error "" "" { target *-*-* } .-3 }
    println!("{} {} {}", 1, 2);
// { dg-error "" "" { target *-*-* } .-1 }
}

