#/bin/bash
BUILD_DIR="gccrs-build-travis"
rm -rf $BUILD_DIR
mkdir $BUILD_DIR
cd $BUILD_DIR
../configure \
    --enable-languages=rust \
    --disable-multilib || exit $?
make || exit $?
cd -

echo
echo "Build Complete"
exit 0
