#include "resource_string.h"
#include "Toolib/enum_cast.h"
#include "Toolib/error.h"
#include "Toolib/trace.h"


namespace res_CmdLine
{
std::string getString(res::ID id)
{
    switch (id)
    {
    case res::ID::invalid: // fall-through
    default:
        too::trace("ERROR") << "res. str. missing handling for id " << too::as_number(id) << " here";
        TOO_ASSERT(false);
        return {};
    }
}

std::string getContentString(const res::ID_alnum&) { throw too::not_implemented{"getContentString"}; }
} // res_CmdLine
