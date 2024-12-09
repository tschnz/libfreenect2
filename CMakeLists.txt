cmake_minimum_required(VERSION 3.21)

set(FREENECT2_MASTER_PROJECT OFF)
if (CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
    set(FREENECT2_MASTER_PROJECT ON)
endif()

option(FREENECT2_STATIC_RUNTIME "Link to static runtime (Windows-only)" ${FREENECT2_MASTER_PROJECT})
option(FREENECT2_TEST "Build the tests" ${FREENECT2_MASTER_PROJECT})
option(FREENECT2_EXAMPLES "Build the examples" ${FREENECT2_MASTER_PROJECT})

if(FREENECT2_STATIC_RUNTIME)
    cmake_policy(SET CMP0091 NEW)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()

# Set the project name
project(freenect2 
    VERSION     0.2.0 
    LANGUAGES   CXX
)

# Options
option(ENABLE_OPENCL "Enable OpenCL support" ON)
option(ENABLE_TINYTHREAD "Use tinythread instead of std::threads" ON)

# TODO test all builds with shared build
option(BUILD_SHARED_LIBS "Build shared (ON) or static (OFF) libraries" OFF)

# TODO test
option(ENABLE_CUDA "Enable CUDA support" OFF)
option(ENABLE_OPENGL "Enable OpenGL support" OFF)
option(ENABLE_VAAPI "Enable VA-API support" OFF)
option(ENABLE_TEGRAJPEG "Enable Tegra HW JPEG support" OFF)
option(ENABLE_OPENNI2 "Enable OpenNI2 support" OFF)
option(ENABLE_PROFILING "Collect profiling stats (memory consuming)" OFF)
option(ENABLE_VT "Enable VideoToolbox support (Mac only)" OFF)
option(BUILD_EXAMPLES "Build examples" OFF)

# Set vars
if(ENABLE_PROFILING)
  set(LIBFREENECT2_WITH_PROFILING 1)
endif()

set(HAVE_CXX11 enabled)

if(ENABLE_TEGRAJPEG)
    set(LIBFREENECT2_WITH_TEGRAJPEG_SUPPORT 1)
    set(LIBFREENECT2_WITH_TURBOJPEG_SUPPORT 0)
else()
    set(LIBFREENECT2_WITH_TEGRAJPEG_SUPPORT 0)
    set(LIBFREENECT2_WITH_TURBOJPEG_SUPPORT 1)
endif()

if(ENABLE_TINYTHREAD)
    set(LIBFREENECT2_THREADING_TINYTHREAD 1)
    set(LIBFREENECT2_THREADING_STDLIB 0)
else()
    set(LIBFREENECT2_THREADING_TINYTHREAD 0)
    set(LIBFREENECT2_THREADING_STDLIB 1)
endif()

# Used throughout the project
set(LIBFREENECT2_WITH_OPENGL_SUPPORT ${ENABLE_OPENGL})
set(LIBFREENECT2_WITH_OPENCL_SUPPORT ${ENABLE_OPENCL})
set(LIBFREENECT2_WITH_CUDA_SUPPORT ${ENABLE_CUDA})
set(LIBFREENECT2_WITH_VT_SUPPORT ${ENABLE_VT})
set(LIBFREENECT2_WITH_VAAPI_SUPPORT ${ENABLE_VAAPI})
set(LIBFREENECT2_WITH_PROFILING ${ENABLE_PROFILING})
set(LIBFREENECT2_WITH_OPENNI2_SUPPORT ${ENABLE_OPENNI2})

message(STATUS "OpenGL support: ${LIBFREENECT2_WITH_OPENGL_SUPPORT}")
message(STATUS "OpenCL support: ${LIBFREENECT2_WITH_OPENCL_SUPPORT}")
message(STATUS "CUDA support: ${LIBFREENECT2_WITH_CUDA_SUPPORT}")
message(STATUS "VT support: ${LIBFREENECT2_WITH_VT_SUPPORT}")
message(STATUS "VA-API support: ${LIBFREENECT2_WITH_VAAPI_SUPPORT}")
message(STATUS "TurboJPEG support: ${LIBFREENECT2_WITH_TURBOJPEG_SUPPORT}")
message(STATUS "TegraJPEG support: ${LIBFREENECT2_WITH_TEGRAJPEG_SUPPORT}")
message(STATUS "Threading stdlib: ${LIBFREENECT2_THREADING_STDLIB}")
message(STATUS "Threading tinythread: ${LIBFREENECT2_THREADING_TINYTHREAD}")
message(STATUS "Profiling: ${LIBFREENECT2_WITH_PROFILING}")
message(STATUS "OpenNI2 support: ${LIBFREENECT2_WITH_OPENNI2_SUPPORT}")

# TODO should we keep those?
set(LIBFREENECT2_WITH_CXX11_SUPPORT 0)
set(LIBFREENECT2_OPENCL_ICD_LOADER_IS_OLD 0)

# Resolve dependencies if using vcpkg
if(EXISTS CACHE{VCPKG_MANIFEST_FILE})
    vcpkg_acquire_dependencies()
endif()

# the library we are creating
add_library(freenect2)
add_library(freenect2::freenect2 ALIAS freenect2)

# Configure a header file to pass some of the CMake settings
configure_file(${CMAKE_SOURCE_DIR}/include/libfreenect2/config.h.in
               ${CMAKE_BINARY_DIR}/include/libfreenect2/config.h)
    
# target_sources(freenect2 PRIVATE
#     ${CMAKE_SOURCE_DIR}/src/openni2/ColorStream.cpp
#     ${CMAKE_SOURCE_DIR}/src/openni2/ColorStream.hpp
#     ${CMAKE_SOURCE_DIR}/src/openni2/DepthStream.cpp
#     ${CMAKE_SOURCE_DIR}/src/openni2/DepthStream.hpp
#     ${CMAKE_SOURCE_DIR}/src/openni2/DeviceDriver.cpp
#     ${CMAKE_SOURCE_DIR}/src/openni2/IrStream.cpp
#     ${CMAKE_SOURCE_DIR}/src/openni2/IrStream.hpp
#     ${CMAKE_SOURCE_DIR}/src/openni2/Registration.cpp
#     ${CMAKE_SOURCE_DIR}/src/openni2/Registration.hpp
#     ${CMAKE_SOURCE_DIR}/src/openni2/Utility.cpp
#     ${CMAKE_SOURCE_DIR}/src/openni2/Utility.hpp
#     ${CMAKE_SOURCE_DIR}/src/openni2/VideoStream.cpp
#     ${CMAKE_SOURCE_DIR}/src/openni2/VideoStream.hpp
# )
    
# target_sources(freenect2 PRIVATE
#     ${CMAKE_SOURCE_DIR}/src/processors/opengl/opengl_depth_packet_processor.cpp
# )
# target_sources(freenect2 PRIVATE
#     ${CMAKE_SOURCE_DIR}/src/processors/opengl/shaders/debug.fs
#     ${CMAKE_SOURCE_DIR}/src/processors/opengl/shaders/debug.vs
#     ${CMAKE_SOURCE_DIR}/src/processors/opengl/shaders/filter1.fs
#     ${CMAKE_SOURCE_DIR}/src/processors/opengl/shaders/filter1.vs
#     ${CMAKE_SOURCE_DIR}/src/processors/opengl/shaders/stage1.fs
#     ${CMAKE_SOURCE_DIR}/src/processors/opengl/shaders/stage1.vs
# )
    
target_sources(freenect2 PRIVATE 
    ${CMAKE_SOURCE_DIR}/src/processors/async_packet_processor.h
    ${CMAKE_SOURCE_DIR}/src/processors/cpu_depth_packet_processor.cpp
    ${CMAKE_SOURCE_DIR}/src/processors/depth_packet_processor.cpp
    ${CMAKE_SOURCE_DIR}/src/processors/depth_packet_processor.h
    ${CMAKE_SOURCE_DIR}/src/processors/depth_packet_stream_parser.cpp
    ${CMAKE_SOURCE_DIR}/src/processors/depth_packet_stream_parser.h
    ${CMAKE_SOURCE_DIR}/src/processors/rgb_packet_processor.cpp
    ${CMAKE_SOURCE_DIR}/src/processors/rgb_packet_processor.h
    ${CMAKE_SOURCE_DIR}/src/processors/rgb_packet_stream_parser.cpp
    ${CMAKE_SOURCE_DIR}/src/processors/rgb_packet_stream_parser.h
    ${CMAKE_SOURCE_DIR}/src/processors/packet_processor.h
)
    
target_sources(freenect2 PRIVATE 
    ${CMAKE_SOURCE_DIR}/src/protocol/command_transaction.h
    ${CMAKE_SOURCE_DIR}/src/protocol/command_transaction.cpp
    ${CMAKE_SOURCE_DIR}/src/protocol/command.h
    ${CMAKE_SOURCE_DIR}/src/protocol/response.h
    ${CMAKE_SOURCE_DIR}/src/protocol/usb_control.h
    ${CMAKE_SOURCE_DIR}/src/protocol/usb_control.cpp
    
    ${CMAKE_SOURCE_DIR}/src/usb/event_loop.h
    ${CMAKE_SOURCE_DIR}/src/usb/event_loop.cpp
    ${CMAKE_SOURCE_DIR}/src/usb/transfer_pool.h
    ${CMAKE_SOURCE_DIR}/src/usb/transfer_pool.cpp
    
    ${CMAKE_SOURCE_DIR}/src/allocator.cpp
    ${CMAKE_SOURCE_DIR}/src/allocator.h
    ${CMAKE_SOURCE_DIR}/src/data_callback.h
    ${CMAKE_SOURCE_DIR}/src/frame_listener_impl.cpp
    ${CMAKE_SOURCE_DIR}/src/libfreenect2.cpp
    ${CMAKE_SOURCE_DIR}/src/logging.cpp
    ${CMAKE_SOURCE_DIR}/src/logging.h
    ${CMAKE_SOURCE_DIR}/src/packet_pipeline.cpp
    ${CMAKE_SOURCE_DIR}/src/registration.cpp
    ${CMAKE_SOURCE_DIR}/src/resource.cpp
    ${CMAKE_SOURCE_DIR}/src/resource.h
)
    
target_sources(freenect2 PRIVATE     
    ${CMAKE_BINARY_DIR}/include/libfreenect2/config.h
    ${CMAKE_SOURCE_DIR}/include/libfreenect2/color_settings.h
    ${CMAKE_SOURCE_DIR}/include/libfreenect2/frame_listener_impl.h
    ${CMAKE_SOURCE_DIR}/include/libfreenect2/led_settings.h
    ${CMAKE_SOURCE_DIR}/include/libfreenect2/libfreenect2.hpp
    ${CMAKE_SOURCE_DIR}/include/libfreenect2/logger.h
    ${CMAKE_SOURCE_DIR}/include/libfreenect2/packet_pipeline.h
    ${CMAKE_SOURCE_DIR}/include/libfreenect2/registration.h
)

# Add include directories
target_include_directories(freenect2 PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

target_compile_features(freenect2 PRIVATE cxx_std_11)

# only set warning options if we are building as a root project
# if we are building as part of another project, we'll defer to
# whatever warning levels they have already set
if (FREENECT2_MASTER_PROJECT)
    # if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    #     # enable most warnings and turn them into errors
    #     target_compile_options(freenect2 PRIVATE /W4)
    #     target_compile_options(freenect2 PRIVATE /WX)
    # else()
    #     include(cmake/target_supported_compile_options.cmake)

    #     target_supported_compile_options(freenect2 PRIVATE -Wall)
    #     target_supported_compile_options(freenect2 PRIVATE -Wextra)
    #     target_supported_compile_options(freenect2 PRIVATE -Wdeprecated)
    #     target_supported_compile_options(freenect2 PRIVATE -Wdocumentation)

    #     target_supported_compile_options(freenect2 PRIVATE -Werror)
    # endif()
endif()

# Generate export header for the library
INCLUDE(GenerateExportHeader)
GENERATE_EXPORT_HEADER(freenect2
    BASE_NAME libfreenect2
    EXPORT_FILE_NAME ${CMAKE_BINARY_DIR}/include/libfreenect2/export.h
)

include(ExternalProject)
# ------------------------------
# Libusb-1.0
# ------------------------------
find_package(libusb-1.0 QUIET)
if (NOT libusb-1.0_FOUND)
    message(STATUS "libusb-1.0 not found. Building ourselves.")
    ExternalProject_Add(
        libusb
        GIT_REPOSITORY https://github.com/libusb/libusb-cmake.git
        GIT_TAG main
        PREFIX ${CMAKE_BINARY_DIR}/libusb
        CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}/libusb/install
    )
endif()
add_dependencies(freenect2 libusb)
target_include_directories(freenect2 PRIVATE ${CMAKE_BINARY_DIR}/libusb/install/include)
target_link_libraries(freenect2 PRIVATE libusb-1.0)

# ------------------------------^
# TurboJPEG
# ------------------------------
find_package(turbojpeg QUIET)
if (NOT turbojpeg_FOUND)
    message(STATUS "libturbojpeg not found. Building ourselves.")
    ExternalProject_Add(
        turbojpeg
        GIT_REPOSITORY https://github.com/libjpeg-turbo/libjpeg-turbo.git
        GIT_TAG main
        PREFIX ${CMAKE_BINARY_DIR}/turbojpeg
        CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}/turbojpeg/install
    )
endif()
add_dependencies(freenect2 turbojpeg)
target_include_directories(freenect2 PRIVATE ${CMAKE_BINARY_DIR}/turbojpeg/install/include)
target_link_libraries(freenect2 PRIVATE libturbojpeg)

# ------------------------------^
# Enable OpenCL
# ------------------------------
if(ENABLE_OPENCL)
    find_package(OpenCL QUIET)
    if (NOT OpenCL_FOUND)
        message(STATUS "OpenCL not found. Building ourselves.")
        ExternalProject_Add(
            opencl-sdk
            GIT_REPOSITORY https://github.com/KhronosGroup/OpenCL-SDK.git
            GIT_TAG main
            PREFIX ${CMAKE_BINARY_DIR}/opencl-sdk
            CMAKE_ARGS 
                -DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}/opencl-sdk/install
                -DBUILD_TESTING=0
                -DBUILD_DOCS=0
                -DBUILD_EXAMPLES=0
                -DBUILD_TESTS=0
                -DOPENCL_SDK_BUILD_SAMPLES=0
                -DOPENCL_SDK_TEST_SAMPLES=0
        )
    endif()
    add_dependencies(freenect2 opencl-sdk)

    target_sources(freenect2 PRIVATE
        ${CMAKE_SOURCE_DIR}/src/processors/opencl/opencl_depth_packet_processor.cpp
        ${CMAKE_SOURCE_DIR}/src/processors/opencl/opencl_depth_packet_processor.cl
        ${CMAKE_SOURCE_DIR}/src/processors/opencl/opencl_kde_depth_packet_processor.cpp
        ${CMAKE_SOURCE_DIR}/src/processors/opencl/opencl_kde_depth_packet_processor.cl
    )
    
    target_include_directories(freenect2 PRIVATE ${CMAKE_BINARY_DIR}/opencl-sdk/install/include)
    target_link_libraries(freenect2 PRIVATE OpenCL)
endif()

# ------------------------------^
# Enable CUDA
# ------------------------------
if(ENABLE_CUDA)
    find_package(CUDA QUIET)
    if (NOT CUDA_FOUND)
        message(STATUS "CUDA not found. Downloading headers.")
            ExternalProject_Add(
            cuda
            GIT_REPOSITORY
            GIT_TAG main
            PREFIX ${CMAKE_BINARY_DIR}/cuda
            CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}/cuda/install
        )
    endif()
    add_dependencies(freenect2 cuda)
    
    target_sources(freenect2 PRIVATE
        ${CMAKE_SOURCE_DIR}/src/processors/cuda/cuda_depth_packet_processor.cu
        ${CMAKE_SOURCE_DIR}/src/processors/cuda/cuda_kde_depth_packet_processor.cu
    )

    target_include_directories(freenect2 PRIVATE ${CMAKE_BINARY_DIR}/cuda/install/include)
endif()

install(
    DIRECTORY   include/libfreenect2
    DESTINATION include
)

install(
    FILES       ${CMAKE_BINARY_DIR}/include/libfreenect2/config.h
                ${CMAKE_BINARY_DIR}/include/libfreenect2/export.h
    DESTINATION include/libfreenect2
)

install(
    TARGETS     freenect2
    EXPORT      freenect2-targets
    DESTINATION lib
)

install(
    EXPORT      freenect2-targets
    NAMESPACE   freenect2::
    DESTINATION lib/cmake/freenect2
)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/freenect2/freenect2-config-version.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion
)

export(
    EXPORT      freenect2-targets
    FILE        "${CMAKE_CURRENT_BINARY_DIR}/freenect2/freenect2-targets.cmake"
    NAMESPACE   freenect2::
)

# install(
#     TARGETS    usb-1.0
#     EXPORT     freenect2-targets
#     DESTINATION lib
# )

# install(
#     TARGETS    turbojpeg
#     EXPORT     freenect2-targets
#     DESTINATION lib
# )

configure_file(cmake/freenect2-config.cmake
    "${CMAKE_CURRENT_BINARY_DIR}/freenect2/freenect2-config.cmake"
    COPYONLY
)

install(
    FILES
        cmake/freenect2-config.cmake
        "${CMAKE_CURRENT_BINARY_DIR}/freenect2/freenect2-config-version.cmake"
    DESTINATION lib/cmake/freenect2
)

if (FREENECT2_TEST)
    # add_subdirectory(test)
endif()

if (FREENECT2_EXAMPLES)
    # add_subdirectory(examples)
endif()

if(FREENECT2_DOCS)
    # add_subdirectory(docs)
endif()