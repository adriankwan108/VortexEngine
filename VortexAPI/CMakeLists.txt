cmake_minimum_required(VERSION 3.12)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
    cmake_policy(SET CMP0141 NEW)
    set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

file(GLOB_RECURSE VORTEX_HEADERS "src/Vortex/**.hpp")
file(GLOB_RECURSE VORTEX_SOURCES "src/Vortex/**.cpp")
file(GLOB VORTEX_API "src/**.hpp")

# libaries
add_library(VortexAPI SHARED)
target_sources(VortexAPI PRIVATE
    ${VORTEX_HEADERS}
    ${VORTEX_SOURCES}
    ${VORTEX_API}
    # glm
)

set_target_properties(VortexAPI PROPERTIES VERSION 0.1.1)

# additional include directories (for #include)
target_include_directories(VortexAPI PUBLIC
    # "Engine/src"
    # spdlog, glfw, vulkan ...
    "vendor/spdlog/include"
)

if(MSVC)
  add_definitions(-DVX_BUILD_DLL)
endif()

# Set output directories
set_target_properties(VortexAPI PROPERTIES
RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/bin/Debug-${CMAKE_SYSTEM_NAME}-${CMAKE_SYSTEM_PROCESSOR}/VortexAPI
RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/bin/Release-${CMAKE_SYSTEM_NAME}-${CMAKE_SYSTEM_PROCESSOR}/VortexAPI
RUNTIME_OUTPUT_DIRECTORY_DIST ${CMAKE_BINARY_DIR}/bin/Dist-${CMAKE_SYSTEM_NAME}-${CMAKE_SYSTEM_PROCESSOR}/VortexAPI

# replace dll automatically
RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/bin/Debug-${CMAKE_SYSTEM_NAME}-${CMAKE_SYSTEM_PROCESSOR}/Engine
RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/bin/Release-${CMAKE_SYSTEM_NAME}-${CMAKE_SYSTEM_PROCESSOR}/Engine
RUNTIME_OUTPUT_DIRECTORY_DIST ${CMAKE_BINARY_DIR}/bin/Dist-${CMAKE_SYSTEM_NAME}-${CMAKE_SYSTEM_PROCESSOR}/Engine

RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/${CMAKE_CFG_INTDIR}

# FOLDER ${CMAKE_BINARY_DIR}/Engine
)