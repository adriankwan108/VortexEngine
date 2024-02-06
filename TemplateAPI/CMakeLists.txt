cmake_minimum_required(VERSION 3.12)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
    cmake_policy(SET CMP0141 NEW)
    set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

file(GLOB TEMPLATE_HEADERS "src/Template/**.hpp")
file(GLOB TEMPLATE_SOURCES "src/Template/**.cpp")
file(GLOB TEMPLATE_API "src/**.hpp")

# libaries
add_library(TemplateAPI SHARED)
target_sources(TemplateAPI PRIVATE
    ${TEMPLATE_HEADERS}
    ${TEMPLATE_SOURCES}
    ${TEMPLATE_API}
    # glm
)

set_target_properties(TemplateAPI PROPERTIES VERSION 0.1.1)

# additional include directories (for #include)
target_include_directories(TemplateAPI PRIVATE
    "Engine/src"
    # spdlog, glfw, vulkan ...
)

# Set output directories
set_target_properties(TemplateAPI PROPERTIES
RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/bin/Debug-${CMAKE_SYSTEM_NAME}-${CMAKE_SYSTEM_PROCESSOR}/TemplateAPI
RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/bin/Release-${CMAKE_SYSTEM_NAME}-${CMAKE_SYSTEM_PROCESSOR}/TemplateAPI
RUNTIME_OUTPUT_DIRECTORY_DIST ${CMAKE_BINARY_DIR}/bin/Dist-${CMAKE_SYSTEM_NAME}-${CMAKE_SYSTEM_PROCESSOR}/TemplateAPI

RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/bin/Debug-${CMAKE_SYSTEM_NAME}-${CMAKE_SYSTEM_PROCESSOR}/Engine
RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/bin/Release-${CMAKE_SYSTEM_NAME}-${CMAKE_SYSTEM_PROCESSOR}/Engine
RUNTIME_OUTPUT_DIRECTORY_DIST ${CMAKE_BINARY_DIR}/bin/Dist-${CMAKE_SYSTEM_NAME}-${CMAKE_SYSTEM_PROCESSOR}/Engine

RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/${CMAKE_CFG_INTDIR}

# FOLDER ${CMAKE_BINARY_DIR}/Engine
)