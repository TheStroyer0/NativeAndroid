cmake_minimum_required(VERSION 3.6)

if(NOT ANDROID_NDK_TOOLCHAIN_INCLUDED)
	message(SEND_ERROR "Android toolchain file not included!")
endif()

add_subdirectory(src)



# package AndroidManifest.xml and res+assets
add_custom_command(OUTPUT intermediates/debug.res
	COMMAND aapt package -f -I $ENV{ANDROID_SDK_ROOT}/platforms/android-29/android.jar -M ${CMAKE_SOURCE_DIR}/android/AndroidManifest.xml -A ${CMAKE_SOURCE_DIR}/assets -S ${CMAKE_SOURCE_DIR}/android/res -F intermediates/debug.res --min-sdk-version 14 --target-sdk-version 23
	COMMENT "Packaging Android resources"
	DEPENDS android/AndroidManifest.xml android/res/ assets/)

# build complete unsigned APK
add_custom_command(OUTPUT intermediates/debug.apk.unsigned
	COMMENT "Building APK"
	# copy native libs
	COMMAND mkdir -p lib lib/${ANDROID_ABI}
	COMMAND cp $<TARGET_FILE:native-activity> lib/${ANDROID_ABI}/
	COMMAND cp intermediates/debug.res intermediates/debug.apk.unsigned
	COMMAND aapt add intermediates/debug.apk.unsigned lib/${ANDROID_ABI}/$<TARGET_FILE_NAME:native-activity> > /dev/null
	DEPENDS native-activity intermediates/debug.res)

# sign APK
add_custom_command(OUTPUT debug.apk
	COMMAND apksigner sign --ks $ENV{HOME}/.android/debug.keystore --ks-key-alias androiddebugkey --ks-pass pass:android --out debug.apk intermediates/debug.apk.unsigned
	COMMAND rm -f debug.apk.idsig
	COMMENT "Signing APK"
	DEPENDS intermediates/debug.apk.unsigned)



add_custom_target(assemble_apk COMMENT "Building APK" DEPENDS debug.apk)
add_custom_target(install_apk COMMAND adb install -r debug.apk COMMENT "Installing APK" DEPENDS debug.apk)
# adb shell am start -n org.example.nativeapp/android.app.NativeActivity -S -W && adb logcat -v raw -v color --pid=`adb shell pidof org.example.nativeapp`
