#ifndef RESOURCE_IDS_H_nexfx783hgfy3zv180
#define RESOURCE_IDS_H_nexfx783hgfy3zv180

#include "toolib/assert.h"
#include "toolib/trace.h"
#include <map>
#include <sstream>
#include <stdexcept>


namespace res
{
enum class ID
{
    invalid                 = -1,
    one_before_first_string = invalid, // just a marker, please sort strings lexicographically below
    one_after_last_string,             // just a marker
};


//! Please keep in sync with ResContentID_from_str!
enum class ContentID
{
    invalid                 = -1,
    one_before_first_string = invalid,
    one_after_last_string,
};

using ID_alnum = std::string;

//! Please keep in sync with ContentID!
inline ContentID ResContentID_from_str(const ID_alnum& s)
{
    // clang-format off
    static const std::map<ID_alnum, ContentID> cachedval{
    };
    // clang-format on
    try
    {
        return cachedval.at(s);
    }
    catch (const std::out_of_range&)
    {
        std::stringstream ss;
        ss << "wrong content value, don't know id '" << s << "' (yet)";
        too::trace() << ss.str();
        TOO_ASSERT(false);
        throw std::out_of_range{ss.str()};
    }
    return ContentID::invalid;
}
} // res

#endif
