#include "MinimalQtTestApp_build_config.h"
#include "_gui/interface/resource_string.h"
#include "toolib/assert.h"
#include "toolib/error.h"
#include "toolib/trace.h"
#include <conio.h>
#include <iostream>

namespace
{
int main_detail(int, char*[])
{
    return too::prog_exit_success;
}

void init_tracer()
{
#if IS_DEPLOYMENT_BUILD
#define ENABLE_TRACER ul::tracer::Disabled
#else
#define ENABLE_TRACER ul::tracer::Enabled
#endif
    ul::tracer::init<ENABLE_TRACER>();
#undef ENABLE_TRACER
}
} // namespace

int main(int argc, char* argv[])
{
    init_tracer();
    try
    {
        return main_detail(argc, argv);
    }
    catch (const std::exception& e)
    {
        std::ostringstream ss;
        ss << "exception: " << e.what();
        ul::trace() << ss.str();
        std::cout << ss.str() << "\n";
        UL_ASSERT(false);
    }
    catch (...)
    {
        std::ostringstream ss;
        ss << "unknown error";
        ul::trace() << ss.str();
        std::cout << ss.str() << "\n";
        UL_ASSERT(false);
    }
    return too::prog_exit_failure;
}
