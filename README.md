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
  can be found in subdirs like `5.9.1`.

Set CMake variables:
* `UL_QT5_VERSION` to e.g. `5.9.1`
* `UL_QT_COMPILER_SUBDIR` to e.g. `gcc_64`


## Contributing

### todo

1. Get rid of that nonsense runtime polymorphism, compile-time
suffices.