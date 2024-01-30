cmake_minimum_required(VERSION 3.10)

project(convertRAW2jpg CXX)

# 设置安装目录
set(CMAKE_INSTALL_PREFIX "${PROJECT_BINARY_DIR}/install")
set(CMAKE_SKIP_INSTALL_RPATH FALSE)
set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")

# 要求编译器支持C++11
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Windows 特定选项和标志
if(MSVC)
    add_compile_options(/utf-8)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()


# OpenCV 手动配置部分（保留 find_package 并注释）
if(WIN32)
    # find_package(OpenCV REQUIRED)  # 注释掉以使用自定义路径
    set(OpenCV_DIR "D:/opencv480vc16/build" CACHE PATH "Path to a custom OpenCV build on Windows")
    set(OPENCV_INCLUDE_DIRS "${OpenCV_DIR}/include")
    set(OPENCV_LIB_DIR     "${OpenCV_DIR}/x64/vc16/lib") # 根据实际编译器版本调整

    include_directories(${OPENCV_INCLUDE_DIRS})
    link_directories(${OPENCV_LIB_DIR})

    list(APPEND EXTRA_LIBS opencv_world480.lib) # 根据实际OpenCV库名称调整
elseif(NOT WIN32)
    # Linux 下的手动 OpenCV 配置
    # find_package(OpenCV REQUIRED)  # 注释掉以使用自定义路径
    set(OpenCV_DIR "~/install/opencv4.5.4" CACHE PATH "Path to a custom OpenCV build on Unix-like systems")
    set(OPENCV_INCLUDE_DIRS "${OpenCV_DIR}/include/opencv4")
    set(OPENCV_LIB_DIR     "${OpenCV_DIR}/lib")

    include_directories(${OPENCV_INCLUDE_DIRS})
    link_directories(${OPENCV_LIB_DIR})

    list(APPEND EXTRA_LIBS opencv_world -fopenmp -lpthread -lm) # 根据实际OpenCV库名称和依赖调整
endif()

include_directories(${OpenCV_INCLUDE_DIRS})

# 添加源文件
add_executable(raw2jpg src/raw2jpg.cpp)
add_executable(jpg2raw src/jpg2raw.cpp)


target_link_libraries(raw2jpg PRIVATE ${EXTRA_LIBS})
target_link_libraries(jpg2raw PRIVATE ${EXTRA_LIBS})

# 安装目标及辅助脚本
set(TMP_TEST_DIR ${PROJECT_SOURCE_DIR}/temp_example)
install(TARGETS raw2jpg
        RUNTIME DESTINATION examples
        RUNTIME DESTINATION ${TMP_TEST_DIR})

install(TARGETS jpg2raw
        RUNTIME DESTINATION examples
        RUNTIME DESTINATION ${TMP_TEST_DIR})

# 找到 assets 目录下所有的 raw 文件，并安装到临时目录
FILE(GLOB RAWS assets/raws/*.raw)
FILE(GLOB JPGS assets/jpgs/*.jpg)

install(FILES ${RAWS} DESTINATION ${TMP_TEST_DIR}/test_raws)
install(FILES ${JPGS} DESTINATION ${TMP_TEST_DIR}/test_jpgs)

if (WIN32)
    install(FILES ${PROJECT_SOURCE_DIR}/onekey_windows_batch2jpg.bat
            DESTINATION examples
            DESTINATION ${TMP_TEST_DIR})
    install(FILES ${PROJECT_SOURCE_DIR}/onekey_windows_batch2raw.bat
            DESTINATION examples
            DESTINATION ${TMP_TEST_DIR})
else()
    install(FILES ${PROJECT_SOURCE_DIR}/onekey_ubuntu_batch2jpg.sh
            DESTINATION examples
            DESTINATION ${TMP_TEST_DIR})
    install(FILES ${PROJECT_SOURCE_DIR}/onekey_ubuntu_batch2raw.sh
            DESTINATION examples
            DESTINATION ${TMP_TEST_DIR})
endif(WIN32)



