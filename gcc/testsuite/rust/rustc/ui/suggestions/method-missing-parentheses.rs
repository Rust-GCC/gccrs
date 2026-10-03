fn main() {
    let _ = vec![].into_iter().collect::<usize>;
// { dg-error ".E0615." "" { target *-*-* } .-1 }
// { dg-error ".E0615." "" { target *-*-* } .-2 }
}

