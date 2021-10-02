cmake_minimum_required(VERSION 3.4)

add_library(native_app_glue STATIC
    ${ANDROID_NDK}/sources/android/native_app_glue/android_native_app_glue.c)

target_include_directories(native_app_glue PRIVATE
	${ANDROID_NDK}/sources/android/native_app_glue)



add_library(native-activity SHARED main.cpp)

target_include_directories(native-activity PRIVATE
	${ANDROID_NDK}/sources/android/native_app_glue)

set_property(TARGET native-activity APPEND_STRING PROPERTY LINK_FLAGS " -u ANativeActivity_onCreate")
# or with CMake 3.13: target_link_options(native-activity PUBLIC -u ANativeActivity_onCreate)



target_link_libraries(native-activity
	android
	native_app_glue
	# EGL
	# GLESv2
	log)
