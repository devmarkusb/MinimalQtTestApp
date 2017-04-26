#include "_gui/interface/resource_string.h"
#include "MinimalQtTestApp_build_config.h"
#include "Toolib/assert.h"
#include "Toolib/error.h"
#include "Toolib/trace.h"
#include <conio.h>
#include <iostream>


namespace
{
int main_detail(int, char* []) { return too::prog_exit_success; }

void init_tracer()
{
#if IS_DEPLOYMENT_BUILD
#define ENABLE_TRACER too::tracer::Disabled
#else
#define ENABLE_TRACER too::tracer::Enabled
#endif
    too::tracer::init<ENABLE_TRACER>();
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
        too::trace() << ss.str();
        std::cout << ss.str() << "\n";
        TOO_ASSERT(false);
    }
    catch (...)
    {
        std::ostringstream ss;
        ss << "unknown error";
        too::trace() << ss.str();
        std::cout << ss.str() << "\n";
        TOO_ASSERT(false);
    }
    return too::prog_exit_failure;
}
