#pragma once
#include <string>
#include <sys/stat.h>

namespace nexus::utils::path {

inline bool CreateDirectoryRecursive(const std::string& path) {
    size_t pos = 0;
    std::string dir;

    if (path[path.length() - 1] != '/') {
        // Добавляем trailing slash
        dir = path + "/";
    } else {
        dir = path;
    }

    while ((pos = dir.find_first_of('/', pos)) != std::string::npos) {
        std::string subdir = dir.substr(0, pos);
        if (!subdir.empty() && subdir != "." && subdir != "..") {
            if (mkdir(subdir.c_str(), 0755) != 0) {
                if (errno != EEXIST) {
                    return false; // Ошибка создания директории
                }
            }
        }
        pos++;
    }

    return true;
}

inline std::string GetDirectory(const std::string& filepath) {
    const size_t pos = filepath.find_last_of('/');
    if (pos != std::string::npos) {
        return filepath.substr(0, pos);
    }
    return "";
}

inline bool EnsureDirectoryExists(const std::string& filepath) {
    std::string dir = GetDirectory(filepath);
    if (!dir.empty()) {
        return CreateDirectoryRecursive(dir);
    }
    return true; // Файл в текущей директории
}

} // namespace nexus::utils::path