cmake_minimum_required(VERSION 3.12)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
    cmake_policy(SET CMP0141 NEW)
    set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

file(GLOB ENGINE_HEADERS "src/**.hpp")
file(GLOB ENGINE_SOURCES "src/**.cpp")

# Our Application
add_executable(Engine)
target_sources(Engine PRIVATE
${ENGINE_HEADERS}
${ENGINE_SOURCES}
)

set_target_properties(Engine PROPERTIES VERSION 0.1.1)

# Set output directories
# set_target_properties(Engine PROPERTIES
# RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/bin/Debug-${CMAKE_SYSTEM_NAME}-${CMAKE_SYSTEM_PROCESSOR}/Engine
# RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/bin/Release-${CMAKE_SYSTEM_NAME}-${CMAKE_SYSTEM_PROCESSOR}/Engine
# RUNTIME_OUTPUT_DIRECTORY_DIST ${CMAKE_BINARY_DIR}/bin/Dist-${CMAKE_SYSTEM_NAME}-${CMAKE_SYSTEM_PROCESSOR}/Engine
# RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/${CMAKE_CFG_INTDIR}
# )
