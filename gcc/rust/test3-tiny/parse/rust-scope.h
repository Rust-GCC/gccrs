#ifndef RUST_SCOPE_H
#define RUST_SCOPE_H

#include <map>
#include <string>

namespace Rust {
    // Map of strings (identifiers) to SymbolPtrs
    struct SymbolMapping {
      public:
        // Inserts a new Symbol into the map using its name as the key. Also checks name is unique.
        void insert(SymbolPtr s);
        // Returns the mapped Symbol for the given string. May return a null Symbol.
        SymbolPtr get(const std::string& str) const;

      private:
        typedef std::map<std::string, SymbolPtr> Map;
        Map map;
    };

    class Scope {};
}

#endif // RUST_SCOPE_H