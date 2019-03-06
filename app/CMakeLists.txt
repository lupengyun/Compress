

cmake_minimum_required(VERSION 3.4.1)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/jniLibs/${ANDROID_ABI})

add_library(compress-lib SHARED src/main/cpp/compress-lib.cpp)

include_directories(src/main/cpp/include)

add_library(jpeg SHARED IMPORTED)

set_target_properties(jpeg PROPERTIES IMPORTED_LOCATION ${PROJECT_SOURCE_DIR}/src/main/jniLibs/${ANDROID_ABI}/libjpeg.so)

find_library(log-lib log)

target_link_libraries(compress-lib log jpeg jnigraphics)