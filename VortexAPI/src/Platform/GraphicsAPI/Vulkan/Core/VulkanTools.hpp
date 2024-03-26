#pragma once

#include "VortexPCH.hpp"
#include "Vortex/Core.hpp"


// Default fence timeout in nanoseconds
#define DEFAULT_FENCE_TIMEOUT 100000000000

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)

#define VK_CHECK_RESULT(f)                                                                                \
{                                                                                                        \
    VkResult res = (f);                                                                                    \
    std::string fileName = __FILE__;                                                                       \
    std::string errorString = "Fatal: VkResult is not success in " + fileName + " at " +TOSTRING(__LINE__) + " line";\
    VX_CORE_ASSERT(res == VK_SUCCESS, errorString);                                                             \
}
