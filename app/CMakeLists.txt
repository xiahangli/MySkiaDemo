cmake_minimum_required(VERSION 3.4.1)

aux_source_directory(src/main/cpp                                                srcs)
aux_source_directory(src/main/cpp/source                                         srcs)
aux_source_directory(src/main/cpp/common                                         srcs)
aux_source_directory(src/main/cpp/gles                                         srcs)
aux_source_directory(src/main/cpp/gles/shape                                         srcs)
aux_source_directory(src/main/cpp/skia                                         srcs)


# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.

add_library( # Sets the name of the library.
             native-lib

             # Sets the library as a shared library.
             SHARED

             # Provides a relative path to your source file(s).
             ${srcs} )

# Searches for a specified prebuilt library and stores the path as a
# variable. Because CMake includes system libraries in the search path by
# default, you only need to specify the name of the public NDK library
# you want to add. CMake verifies that the library exists before
# completing its build.

include_directories(
            ${CMAKE_SOURCE_DIR}/src/main/cpp/3rd
            ${CMAKE_SOURCE_DIR}/src/main/cpp/3rd/skia
            ${CMAKE_SOURCE_DIR}/src/main/cpp/3rd/skia/atlastext
            ${CMAKE_SOURCE_DIR}/src/main/cpp/3rd/skia/c
            ${CMAKE_SOURCE_DIR}/src/main/cpp/3rd/skia/codec
            ${CMAKE_SOURCE_DIR}/src/main/cpp/3rd/skia/config
            ${CMAKE_SOURCE_DIR}/src/main/cpp/3rd/skia/core
            ${CMAKE_SOURCE_DIR}/src/main/cpp/3rd/skia/effects
            ${CMAKE_SOURCE_DIR}/src/main/cpp/3rd/skia/encode
            ${CMAKE_SOURCE_DIR}/src/main/cpp/3rd/skia/gpu
            ${CMAKE_SOURCE_DIR}/src/main/cpp/3rd/skia/pathops
            ${CMAKE_SOURCE_DIR}/src/main/cpp/3rd/skia/ports
            ${CMAKE_SOURCE_DIR}/src/main/cpp/3rd/skia/private
            ${CMAKE_SOURCE_DIR}/src/main/cpp/3rd/skia/svg
            ${CMAKE_SOURCE_DIR}/src/main/cpp/3rd/skia/utils
            ${CMAKE_SOURCE_DIR}/src/main/cpp/3rd/skia/views
            ${CMAKE_SOURCE_DIR}/src/main/cpp/3rd/v8
            ${CMAKE_SOURCE_DIR}/src/main/cpp/source

            ${CMAKE_SOURCE_DIR}/src/main/cpp/common
            ${CMAKE_SOURCE_DIR}/src/main/cpp/gles
            ${CMAKE_SOURCE_DIR}/src/main/cpp/gles/shape
             )

find_library( # Sets the name of the path variable.
              log-lib

              # Specifies the name of the NDK library that
              # you want CMake to locate.
              log )

find_library(z-lib z)

# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in this
# build script, prebuilt third-party libraries, or system libraries.

target_link_libraries( # Specifies the target library.
                       native-lib

                       # Links the target library to the log library
                       # included in the NDK.
                       ${log-lib} android EGL GLESv2 jnigraphics

                      # for unzip
                      ${z-lib}

                      # skia and v8
                      ${CMAKE_SOURCE_DIR}/libs/${ANDROID_ABI}/libskia.a
#                      ${CMAKE_SOURCE_DIR}/libs/${ANDROID_ABI}/libv8_base.a
#                      ${CMAKE_SOURCE_DIR}/libs/${ANDROID_ABI}/libv8_libplatform.a
#                      ${CMAKE_SOURCE_DIR}/libs/${ANDROID_ABI}/libv8_libbase.a
#                      ${CMAKE_SOURCE_DIR}/libs/${ANDROID_ABI}/libv8_init.a
#                      ${CMAKE_SOURCE_DIR}/libs/${ANDROID_ABI}/libv8_initializers.a
#                      ${CMAKE_SOURCE_DIR}/libs/${ANDROID_ABI}/libv8_libsampler.a
#                      ${CMAKE_SOURCE_DIR}/libs/${ANDROID_ABI}/libv8_snapshot.a
        )