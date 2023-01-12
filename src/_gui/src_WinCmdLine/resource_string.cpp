#include "resource_string.h"
#include "toolib/enum_cast.h"
#include "toolib/error.h"
#include "toolib/trace.h"

namespace res_CmdLine
{
std::string getString(res::ID id)
{
    switch (id)
    {
        case res::ID::invalid: // fall-through
        default:
            ul::trace("ERROR") << "res. str. missing handling for id " << too::as_number(id) << " here";
            UL_ASSERT(false);
            return {};
    }
}

std::string getContentString(const res::ID_alnum&)
{
    throw too::not_implemented{"getContentString"};
}
} // namespace res_CmdLine
