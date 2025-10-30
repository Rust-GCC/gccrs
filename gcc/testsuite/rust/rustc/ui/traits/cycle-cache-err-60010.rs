// Test that we properly detect the cycle amongst the traits
// here and report an error.

use std::panic::RefUnwindSafe;

trait Database {
    type Storage;
}
trait HasQueryGroup {}
trait Query<DB> {
    type Data;
}
trait SourceDatabase {
    fn parse(&self) {
        loop {}
    }
}

struct ParseQuery;
struct RootDatabase {
    _runtime: Runtime<RootDatabase>,
}
struct Runtime<DB: Database> {
    _storage: Box<DB::Storage>,
}
struct SalsaStorage {
    _parse: <ParseQuery as Query<RootDatabase>>::Data,
}

impl Database for RootDatabase {
    // This would also be an error if we didn't abort compilation on the error
    // above.
    type Storage = SalsaStorage;
}
impl HasQueryGroup for RootDatabase {}
impl<DB> Query<DB> for ParseQuery
where
    DB: SourceDatabase,
    DB: Database,
{
    type Data = RootDatabase;
}
impl<T> SourceDatabase for T
where
    T: RefUnwindSafe,
    T: HasQueryGroup,
{
}

pub(crate) fn goto_implementation(db: &RootDatabase) -> u32 {
    // This is not satisfied:
    //
    // - `RootDatabase: SourceDatabase`
    //   - requires `RootDatabase: RefUnwindSafe` + `RootDatabase: HasQueryGroup`
    // - `RootDatabase: RefUnwindSafe`
    //   - requires `Runtime<RootDatabase>: RefUnwindSafe`
    // - `Runtime<RootDatabase>: RefUnwindSafe`
    //   - requires `DB::Storage: RefUnwindSafe` (`SalsaStorage: RefUnwindSafe`)
    // - `SalsaStorage: RefUnwindSafe`
    //    - requires `<ParseQuery as Query<RootDatabase>>::Data: RefUnwindSafe`,
    //      which means `ParseQuery: Query<RootDatabase>`
    // - `ParseQuery: Query<RootDatabase>`
    //    - requires `RootDatabase: SourceDatabase`,
    // - `RootDatabase: SourceDatabase` is already on the stack, so we have a
    //   cycle with non-coinductive participants
    //
    // we used to fail to report an error here because we got the
    // caching wrong.
    SourceDatabase::parse(db);
// { dg-error ".E0275." "" { target *-*-* } .-1 }
    22
}

fn main() {}

