#include "resource_string.h"
#include "_gui/src_Qt/resource_string/resource_string.h"
#include "ul/ul.h"

namespace res {
std::string getString(res::ID id) {
    return res_Qt::getString(id);
}

std::string getContentString(const res::ID_alnum& s) {
    return res_Qt::getContentString(s);
}

//! Expects in non-empty. Expects replacements to be of at least the count of occurrences of "%s" within in.
std::string fillResStrPlaceholders(const std::string& in, const std::vector<std::string>& replacements) {
    UL_EXPECT(!in.empty());
    std::string ret{in};
    ul::str::replace_all(ret, "%s", replacements);
    return ret;
}
} // namespace res
