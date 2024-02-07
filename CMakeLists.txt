cmake_minimum_required(VERSION 3.13)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
    cmake_policy(SET CMP0141 NEW)
    set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

# set the project name
project(VortexEngine VERSION 0.1.1)

# set C++ version
if (CMAKE_VERSION VERSION_GREATER 3.13)
    # set_property(TARGET TemplateEngine PROPERTY CXX_STANDARD 20)
    set(CMAKE_CXX_STANDARD 20)
endif()
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

# add_compile_options(-fsanitize=address)
# add_link_options(-fsanitize=address)
add_subdirectory(VortexAPI)
add_subdirectory(Engine)

# Turn on any compiler-specific flags
if (WIN32)
	# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /WX")
else()
	# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror")
	# if ("$ENV{SANITIZE}" STREQUAL "ON")
		# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address") # fast memory error detector
	# endif()
	
	if(APPLE)
		if(CMAKE_GENERATOR STREQUAL Xcode)
			# set(CMAKE_XCODE_GENERATE_SCHEME YES)
			set(CMAKE_XCODE_ATTRIBUTE_GCC_GENERATE_DEBUGGING_SYMBOLS[variant=Debug] "YES")
			set(CMAKE_XCODE_ATTRIBUTE_GCC_GENERATE_DEBUGGING_SYMBOLS[variant=MinSizeRel] "NO")
			set(CMAKE_XCODE_ATTRIBUTE_GCC_GENERATE_DEBUGGING_SYMBOLS[variant=RelWithDebInfo] "NO")
			set(CMAKE_XCODE_ATTRIBUTE_GCC_GENERATE_DEBUGGING_SYMBOLS[variant=Release] "YES")
			set(CMAKE_XCODE_ATTRIBUTE_GCC_GENERATE_DEBUGGING_SYMBOLS[variant=Dist] "YES")

			# set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/build/Debug)
			# set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/build/Release)
			# set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/build/Debug)
			# set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/build/Release)
			# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/build/Debug)
			# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/build/Release)
		endif()
	endif()
endif()

set(CMAKE_USE_RELATIVE_PATHS ON)

# Add support for additional build configurations
set(CMAKE_CONFIGURATION_TYPES "Debug;Release;Dist" CACHE STRING "" FORCE)
# set(EXCLUDE_FROM_ALL true)
# set(CMAKE_SUPPRESS_REGENERATION true)

# set_property(GLOBAL PROPERTY USE_FOLDERS ON)
# set(PREDEFINED_TARGETS_FOLDER "CustomTargets")

# set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})

target_include_directories(Engine PRIVATE
	"VortexAPI/src"
)

target_link_libraries(Engine PRIVATE
	VortexAPI
)

set_property(DIRECTORY ${CMAKE_BINARY_DIR} PROPERTY VS_STARTUP_PROJECT Engine)

add_compile_options(-D VX_BUILD_DLL)

# Generate Xcode project
# set(CMAKE_XCODE_SCHEME_EXECUTABLE Engine)
# set(XCODE_SCHEME "Engine" CACHE STRING "Default Xcode scheme")