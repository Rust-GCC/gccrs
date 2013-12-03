#/bin/bash
BUILD_DIR="gccrs-build-travis"
rm -rf $BUILD_DIR
mkdir $BUILD_DIR
cd $BUILD_DIR
../configure --enable-languages=rust --disable-multilib --disable-bootstrap
make
cd -
