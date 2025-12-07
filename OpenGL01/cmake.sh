#/bin/sh

rm -rf build
cmake -B build -S . -DVCPKG_ROOT="~/Downloads/vcpkg-master"
# make -C build