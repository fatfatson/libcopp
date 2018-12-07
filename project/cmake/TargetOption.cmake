﻿# 设置实际的默认编译输出目录，为防止呗外部模块设置，所以要先判定一下

if (NOT CMAKE_ARCHIVE_OUTPUT_DIRECTORY)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib${PLATFORM_SUFFIX}")
endif()

if (NOT CMAKE_LIBRARY_OUTPUT_DIRECTORY)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY})
endif()

if (NOT CMAKE_RUNTIME_OUTPUT_DIRECTORY)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin")
endif()

set(PROJECT_TARGET_INSTALL_EXECUTABLE_DIR "bin")
set(PROJECT_TARGET_INSTALL_ARCHIVE_DIR "lib")
set(PROJECT_TARGET_INSTALL_LIBRARY_DIR "lib")

# set(EXECUTABLE_OUTPUT_PATH "${PROJECT_BINARY_DIR}/bin")

link_directories (${CMAKE_ARCHIVE_OUTPUT_DIRECTORY})