#/bin/sh

rm -rf build
cmake -B build -S . -DVCPKG_ROOT="path-to-vcpkg"