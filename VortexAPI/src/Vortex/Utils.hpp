#include <fstream>
#include <filesystem>
#include <algorithm>
#include "VortexPCH.hpp"

namespace VX
{
    namespace Utils
    {
        static std::string GetProjectRootPath()
        {
            std::filesystem::path absolutePath = std::filesystem::current_path();

            std::filesystem::path tempPath = absolutePath.root_path();

            for (const auto& path : absolutePath)
            {
                if (!path.has_stem())
                {
                    continue;
                }

                tempPath += path;
    #if defined(__APPLE__)
                tempPath += "/";
    #elif defined(_MSC_VER)
                tempPath += "\\";
    #endif

                if (path.stem() == "VortexEngine")
                {
                    break;
                }
            }

            return tempPath.string();
        }

        static std::string AbsolutePath(const std::string& inputPath)
        {
            //std::string path = std::move(inputPath);
            std::string path = inputPath;
    #if defined(__APPLE__)
            return GetProjectRootPath() + path;
    #elif defined(_MSC_VER)
            std::replace(path.begin(), path.end(), '/', '\\');
            return GetProjectRootPath() + "\\" + path;
    #endif
        }

        static std::vector<uint32_t> readFile(const std::string& filename) {
            
            std::ifstream file(AbsolutePath(filename), std::ios::ate | std::ios::binary);
            std::vector<uint32_t> buffer(0);
            if (!file.good())
            {
                VX_CORE_WARN("Utils: File ({0}) not exists.", filename);
                return buffer;
            }

            if (!file.is_open()) {
                VX_CORE_ERROR("Utils: Failed to open file ({0})!", filename);
                throw std::runtime_error("failed to open file!");
            }
            
            size_t fileSize = static_cast<size_t>(file.tellg());
            buffer.resize(fileSize / sizeof(uint32_t));
            
            file.seekg(0);
            file.read(reinterpret_cast<char*>(buffer.data()), fileSize);
            file.close();

            return buffer;
        }

    template <typename T>
    size_t CalculateMemorySize(const std::vector<T>& v)
    {
        return sizeof(v[0]) * v.size();
    }
#define MEM_SIZE(...)    ::VX::Utils::CalculateMemorySize(__VA_ARGS__)
    }
}
