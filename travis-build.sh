#/bin/bash
BUILD_DIR="gccrs-build-travis"
rm -rf $BUILD_DIR
mkdir $BUILD_DIR
./contrib/download_prerequisites || exit $?
cd $BUILD_DIR
../configure \
    --enable-languages=rust \
    --disable-multilib \
    --disable-bootstrap || exit $?
make || exit $?
cd -

echo
echo "Build Complete"
exit 0
