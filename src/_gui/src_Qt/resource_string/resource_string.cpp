#include "resource_string.h"
#include "Toolib/enum_cast.h"
#include "Toolib/error.h"
#include "Toolib/trace.h"
#include "uiwrap/string/impl_Qt/StringConvert_Qt.h"
#include "Toolib/PPDefs/MSVC/SUPPRESS_WARNINGS_EXTERNAL_BEGIN"
#include <QString>
#include <QTranslator>
#include "Toolib/PPDefs/MSVC/SUPPRESS_WARNINGS_EXTERNAL_END"


namespace res_Qt
{
QString getQString(res::ID id)
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

std::string getString(res::ID id)
{
    return uiw::implQt::qs2s(getQString(id));
}

QString getContentQString(const res::ID_alnum&) { throw too::not_implemented{"getContentQtString"}; }

#include "Toolib/PPDefs/MSVC/SUPPRESS_WARNING_4702_BEGIN"
std::string getContentString(const res::ID_alnum& id)
{
    return uiw::implQt::qs2s(getContentQString(id));
}
#include "Toolib/PPDefs/MSVC/SUPPRESS_WARNING_END"
} // res_Qt
