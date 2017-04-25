#include "resource_string.h"
#include "../src_WinCmdLine/resource_string.h"
#include "Toolib/assert.h"
#include "Toolib/string/string_misc.h"


namespace res
{
std::string getString(res::ID id) { return res_CmdLine::getString(id); }

std::string getContentString(const res::ID_alnum& s) { return res_CmdLine::getContentString(s); }

//! Expects \param in non-empty. Expects \params replacements to be of at least the
//! count of occurrences of "%s" within \param in.
std::string fillResStrPlaceholders(const std::string& in, const std::vector<std::string>& replacements)
{
    TOO_EXPECT(!in.empty());
    std::string ret{in};
    too::str::replaceAll(ret, "%s", replacements);
    return ret;
}
}
