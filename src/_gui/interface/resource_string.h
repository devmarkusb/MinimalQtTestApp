#ifndef RESOURCE_STRING_H_x2gf893zxrnmyeiowrhq
#define RESOURCE_STRING_H_x2gf893zxrnmyeiowrhq

#include "resource_ids.h"
#include <string>
#include <vector>


namespace res
{
std::string getString(res::ID id);
std::string getContentString(const res::ID_alnum& s);
//! Replaces occurrences of %s within \param in by \param replacemens in order.
//! So far escaping is not supported.
std::string fillResStrPlaceholders(const std::string& in, const std::vector<std::string>& replacements);
} // namespace res

#endif
