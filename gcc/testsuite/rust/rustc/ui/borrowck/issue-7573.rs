pub struct CrateId {
    local_path: String,
    junk: String,
}

impl CrateId {
    fn new(s: &str) -> CrateId {
        CrateId { local_path: s.to_string(), junk: "wutevs".to_string() }
    }
}

pub fn remove_package_from_database() {
    let mut lines_to_use: Vec<&CrateId> = Vec::new();
// { dg-note "" "" { target *-*-* } .-1 }
    let push_id = |installed_id: &CrateId| {
// { dg-note "" "" { target *-*-* } .-1 }
        lines_to_use.push(installed_id);
// { dg-error ".E0521." "" { target *-*-* } .-1 }
// { dg-note ".E0521." "" { target *-*-* } .-2 }
    };
    list_database(push_id);

    for l in &lines_to_use {
        println!("{}", l.local_path);
    }
}

pub fn list_database<F>(mut f: F)
where
    F: FnMut(&CrateId),
{
    let stuff = ["foo", "bar"];

    for l in &stuff {
        f(&CrateId::new(*l));
    }
}

pub fn main() {
    remove_package_from_database();
}

