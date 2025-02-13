#include "resource_string.h"
#include "ul/ul.h"

#include "ul/warnings.h"
UL_PRAGMA_WARNINGS_PUSH_AND_DISABLE_ALL_MSVC
#include <QString>
#include <QTranslator>
UL_PRAGMA_WARNINGS_POP

namespace res_Qt {
namespace {
[[maybe_unused]] QString getQString(res::ID id) {
    switch (id) {
        case res::ID::invalid: // fall-through
        default:
            ul::trace("ERROR") << "res. str. missing handling for id " << ul::as_number(id) << " here";
            UL_ASSERT(false);
            return {};
    }
}

[[maybe_unused]] QString getContentQString(const res::ID_alnum&) {
    throw ul::NotImplemented{"getContentQtString"};
}
} // namespace

std::string getString(res::ID) {
    throw ul::NotImplemented{"getString"};
}

UL_PRAGMA_WARNINGS_PUSH
UL_WARNING_DISABLE_MSVC(4702)

std::string getContentString(const res::ID_alnum&) {
    throw ul::NotImplemented{"getContentString"};
}

UL_PRAGMA_WARNINGS_POP
} // namespace res_Qt
