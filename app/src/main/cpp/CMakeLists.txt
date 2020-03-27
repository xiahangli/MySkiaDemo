# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.4.1)

# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.

add_library( # Sets the name of the library.
        native-lib

        # Sets the library as a shared library.
        SHARED

        # Provides a relative path to your source file(s).
        native-lib.cpp)

# Searches for a specified prebuilt library and stores the path as a
# variable. Because CMake includes system libraries in the search path by
# default, you only need to specify the name of the public NDK library
# you want to add. CMake verifies that the library exists before
# completing its build.


set(ANDROID_NDK_REVISION 16)

set(CMAKE_VERBOSE_MAKEFILE on)

set(ANDROID_STL "gnustl_shared")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11 -fexceptions -frtti")

set(libs "../jniLibs")
message(libs)

include_directories(src/main/cpp/include/)

include_directories(${CMAKE_SOURCE_DIR}/src/main/cpp/include/android)

include_directories(${CMAKE_SOURCE_DIR}/src/main/cpp/include/atlastext)

include_directories(${CMAKE_SOURCE_DIR}/src/main/cpp/include/c)

include_directories(${CMAKE_SOURCE_DIR}/src/main/cpp/include/codec)

include_directories(${CMAKE_SOURCE_DIR}/src/main/cpp/include/config)

include_directories(${CMAKE_SOURCE_DIR}/src/main/cpp/include/core)

include_directories(${CMAKE_SOURCE_DIR}/src/main/cpp/include/effects)

include_directories(${CMAKE_SOURCE_DIR}/src/main/cpp/include/encode)

include_directories(${CMAKE_SOURCE_DIR}/src/main/cpp/include/gpu)

include_directories(${CMAKE_SOURCE_DIR}/src/main/cpp/include/pathops)

include_directories(${CMAKE_SOURCE_DIR}/src/main/cpp/include/ports)

include_directories(${CMAKE_SOURCE_DIR}/src/main/cpp/include/private)

include_directories(${CMAKE_SOURCE_DIR}/src/main/cpp/include/svg)

include_directories(${CMAKE_SOURCE_DIR}/src/main/cpp/include/utils)

include_directories(${CMAKE_SOURCE_DIR}/src/main/cpp/include/views)

add_library(libskia_eastedu SHARED IMPORTED )

set_target_properties(libskia_eastedu PROPERTIES

    IMPORTED_LOCATION "${libs}/${ANDROID_ABI}/libskia.so")

add_library(eskia
            SHARED
            eskia.cpp )

#find_library( android-lib android)

target_link_libraries(eskia
        libskia_eastedu
        ${android-lib}
    )