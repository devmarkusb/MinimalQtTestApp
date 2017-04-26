#include "resource_string.h"
#ifdef UIW_CHOICE_QT
#include "_gui/src_Qt/resource_string/resource_string.h"
#else
#include "_gui/src_WinCmdLine/resource_string.h"
#endif
#include "Toolib/assert.h"
#include "Toolib/string/string_misc.h"


namespace res
{
std::string getString(res::ID id)
{
#ifdef UIW_CHOICE_QT
    return res_Qt::getString(id);
#else
    return res_CmdLine::getString(id);
#endif
}

std::string getContentString(const res::ID_alnum& s)
{
#ifdef UIW_CHOICE_QT
    return res_Qt::getContentString(s);
#else
    return res_CmdLine::getContentString(s);
#endif
}

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
