cmake_minimum_required(VERSION 3.16)

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

option(BUILD_SHARED_LIBS "Build shared (ON) or static (OFF) libraries" OFF)
option(BUILD_EXAMPLES "Build examples" OFF)
option(ENABLE_OPENCL "Enable OpenCL support" OFF)
option(ENABLE_CUDA "Enable CUDA support" OFF)
option(ENABLE_OPENGL "Enable OpenGL support" OFF)
option(ENABLE_VAAPI "Enable VA-API support" OFF)
option(ENABLE_TEGRAJPEG "Enable Tegra HW JPEG support" OFF)
option(ENABLE_PROFILING "Collect profiling stats (memory consuming)" OFF)

# Set vars
if(ENABLE_PROFILING)
  set(LIBFREENECT2_WITH_PROFILING 1)
endif()

set(HAVE_CXX11 enabled)

# Used throughout the project
set(LIBFREENECT2_WITH_OPENGL_SUPPORT 0)
set(LIBFREENECT2_WITH_OPENCL_SUPPORT 0)
set(LIBFREENECT2_OPENCL_ICD_LOADER_IS_OLD 0)
set(LIBFREENECT2_WITH_CUDA_SUPPORT 0)
set(LIBFREENECT2_WITH_VT_SUPPORT 0)
set(LIBFREENECT2_WITH_VAAPI_SUPPORT 0)
set(LIBFREENECT2_WITH_TURBOJPEG_SUPPORT 1)
set(LIBFREENECT2_WITH_TEGRAJPEG_SUPPORT 0)
set(LIBFREENECT2_THREADING_STDLIB 1)
set(LIBFREENECT2_THREADING_TINYTHREAD 0)
set(LIBFREENECT2_WITH_CXX11_SUPPORT 0)
set(LIBFREENECT2_WITH_PROFILING 0)

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

set(INCLUDE_HEADERS
    ${CMAKE_BINARY_DIR}/include/libfreenect2/config.h
    ${CMAKE_SOURCE_DIR}/include/libfreenect2/color_settings.h
    ${CMAKE_SOURCE_DIR}/include/libfreenect2/frame_listener_impl.h
    ${CMAKE_SOURCE_DIR}/include/libfreenect2/led_settings.h
    ${CMAKE_SOURCE_DIR}/include/libfreenect2/libfreenect2.hpp
    ${CMAKE_SOURCE_DIR}/include/libfreenect2/logger.h
    ${CMAKE_SOURCE_DIR}/include/libfreenect2/packet_pipeline.h
    ${CMAKE_SOURCE_DIR}/include/libfreenect2/registration.h
)
    
set(OPENNI_SOURCES
    ${CMAKE_SOURCE_DIR}/src/openni2/ColorStream.cpp
    ${CMAKE_SOURCE_DIR}/src/openni2/ColorStream.hpp
    ${CMAKE_SOURCE_DIR}/src/openni2/DepthStream.cpp
    ${CMAKE_SOURCE_DIR}/src/openni2/DepthStream.hpp
    ${CMAKE_SOURCE_DIR}/src/openni2/DeviceDriver.cpp
    ${CMAKE_SOURCE_DIR}/src/openni2/IrStream.cpp
    ${CMAKE_SOURCE_DIR}/src/openni2/IrStream.hpp
    ${CMAKE_SOURCE_DIR}/src/openni2/Registration.cpp
    ${CMAKE_SOURCE_DIR}/src/openni2/Registration.hpp
    ${CMAKE_SOURCE_DIR}/src/openni2/Utility.cpp
    ${CMAKE_SOURCE_DIR}/src/openni2/Utility.hpp
    ${CMAKE_SOURCE_DIR}/src/openni2/VideoStream.cpp
    ${CMAKE_SOURCE_DIR}/src/openni2/VideoStream.hpp
)
    
set(PROCESSOR_SOURCES_CUDA_RESOURCES
    ${CMAKE_SOURCE_DIR}/src/processors/cuda/cuda_depth_packet_processor.cu
    ${CMAKE_SOURCE_DIR}/src/processors/cuda/cuda_kde_depth_packet_processor.cu
)
    
set(PROCESSOR_SOURCES_OPENCL
    ${CMAKE_SOURCE_DIR}/src/processors/opencl/opencl_depth_packet_processor.cpp
    ${CMAKE_SOURCE_DIR}/src/processors/opencl/opencl_kde_depth_packet_processor.cpp
)
set(PROCESSOR_SOURCES_OPENCL_RESOURCES
    ${CMAKE_SOURCE_DIR}/src/processors/opencl/opencl_depth_packet_processor.cl
    ${CMAKE_SOURCE_DIR}/src/processors/opencl/opencl_kde_depth_packet_processor.cl
)
    
set(PROCESSOR_SOURCES_OPENGL
    ${CMAKE_SOURCE_DIR}/src/processors/opengl/opengl_depth_packet_processor.cpp
)
set(PROCESSOR_SOURCES_OPENGL_RESOURCES
    ${CMAKE_SOURCE_DIR}/src/processors/opengl/shaders/debug.fs
    ${CMAKE_SOURCE_DIR}/src/processors/opengl/shaders/debug.vs
    ${CMAKE_SOURCE_DIR}/src/processors/opengl/shaders/filter1.fs
    ${CMAKE_SOURCE_DIR}/src/processors/opengl/shaders/filter1.vs
    ${CMAKE_SOURCE_DIR}/src/processors/opengl/shaders/stage1.fs
    ${CMAKE_SOURCE_DIR}/src/processors/opengl/shaders/stage1.vs
)
    
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
    
target_sources(freenect2 PRIVATE ${INCLUDE_HEADERS})

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
ExternalProject_Add(
    libusb
    GIT_REPOSITORY https://github.com/libusb/libusb-cmake.git
    GIT_TAG main
    PREFIX ${CMAKE_BINARY_DIR}/libusb
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}/libusb/install
)
add_dependencies(freenect2 libusb)
target_include_directories(freenect2 PRIVATE ${CMAKE_BINARY_DIR}/libusb/install/include)
target_link_libraries(freenect2 PRIVATE libusb-1.0)

ExternalProject_Add(
    turbojpeg
    GIT_REPOSITORY https://github.com/libjpeg-turbo/libjpeg-turbo.git
    GIT_TAG main
    PREFIX ${CMAKE_BINARY_DIR}/turbojpeg
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}/turbojpeg/install
)
add_dependencies(freenect2 turbojpeg)
target_include_directories(freenect2 PRIVATE ${CMAKE_BINARY_DIR}/turbojpeg/install/include)
target_link_libraries(freenect2 PRIVATE libturbojpeg)

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