# NativeAndroid

Build Native Android Projects using CMake.

This CMake script does the minimum ammount of work for building native Android applications.



## Setup

1. The Android SDK needs to be installed and the `ANDROID_SDK_ROOT` environment variable set.
2. The Android _build-tools_ have to be added to the `PATH` environment variable, since the CMake script uses `aapt` and `apksigner` to package and sign the apk.
3. The Android _platform-tools_ have to be added to the `PATH` environment variable for the `install_apk` target of the script, since it uses `adb install` to install the apk.
4. The debug key has to be located at `~/.android/debug.keystore` since the script uses this key for signing.



### Setting up the Build Directory

Run the following command, after setting `-DANDROID_ABI` to the preffered ABI (and replacing `ndk-bundle` with the NDK version if needed):
```
cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DCMAKE_TOOLCHAIN_FILE=$ANDROID_SDK_ROOT/ndk-bundle/build/cmake/android.toolchain.cmake -DANDROID_ABI=armeabi-v7a -DANDROID_PLATFORM=android-16 -DCMAKE_SYSTEM_NAME=Android -B build -GNinja
```



### Building the Project

Running the following (or `cmake --build ./ --target assemble_apk`) in the build directory will build the apk.
```
ninja assemble_apk
```
  
Running the target `install_apk` will additionally install the apk.

The app can then be run using  
`` adb shell am start -n org.example.nativeapp/android.app.NativeActivity -S -W && adb logcat -v raw -v color --pid=`adb shell pidof org.example.nativeapp` ``.



## Ideas

This could easily be extended to:
- build Java code
- zipalign the apk
- use a custom key for signing the apk

