#include "system.h" // for gcc_assert - can move to header if needed there

#include "rust-scope.h"

namespace Rust {
    void SymbolMapping::insert(SymbolPtr s) {
        gcc_assert(s != NULL);
        std::pair<Map::iterator, bool> p = map.insert(std::make_pair(s->get_name(), s));

        gcc_assert(p.second);
    }

    SymbolPtr SymbolMapping::get(const std::string& str) const {
        Map::const_iterator it = map.find(str);
        if (it != map.end()) {
            return it->second;
        }

        return SymbolPtr();
    }
}