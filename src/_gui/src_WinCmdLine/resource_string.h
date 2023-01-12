#ifndef RESOURCE_STRING_H_euixgn4fv13ybfz3z9f42zhd4hh
#define RESOURCE_STRING_H_euixgn4fv13ybfz3z9f42zhd4hh

#include "_gui/interface/resource_ids.h"
#include <string>

namespace res_CmdLine
{
std::string getString(res::ID id);
std::string getContentString(const res::ID_alnum& s);
} // namespace res_CmdLine

#endif
