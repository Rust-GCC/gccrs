fn main () {
    let sr: Vec<(u32, _, _) = vec![];
// { dg-error "" "" { target *-*-* } .-1 }
    let sr2: Vec<(u32, _, _)> = sr.iter().map(|(faction, th_sender, th_receiver)| {}).collect();
// { dg-error ".E0277." "" { target *-*-* } .-1 }
}

