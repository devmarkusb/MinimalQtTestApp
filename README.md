# MinimalQtTestApp

[![build](https://github.com/devmarkusb/MinimalQtTestApp/actions/workflows/build.yml/badge.svg)](https://github.com/devmarkusb/MinimalQtTestApp/actions/workflows/build.yml)
[![build-android](https://github.com/devmarkusb/MinimalQtTestApp/actions/workflows/build-android.yml/badge.svg)](https://github.com/devmarkusb/MinimalQtTestApp/actions/workflows/build-android.yml)

A minimal Qt app. Mainly for tests of platform
independence, concerning build, deploy and run.

## Usage and start of the qt_qml_extensions_testapp

Set environment variables:
* `export dev_sdk_path=/path-to-sdks/` where versions of Qt reside
  (and possibly also boost).
* `export dev_qt_base=$dev_sdk_path/qt_linux` (e.g.) where Qt versions
  can be found in subdirs like `6.9.3`.

Set CMake variables:
* `-DCMAKE_PREFIX_PATH=$dev_sdk_path/Qt/6.9.3/gcc_64` (e.g.)

## Contributing

### todo

1. Get rid of that nonsense runtime polymorphism, compile-time
suffices.