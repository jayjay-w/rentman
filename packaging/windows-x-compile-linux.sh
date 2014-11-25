#Creates windows binaries

BUILD_DIR=$1
CODE_PATH=$2
MINGW_DIR=/usr/i686-w64-mingw32/bin
QT_LIBS_DIR=/usr/i686-w64-mingw32/lib/qt/plugins

OUT_PATH=$BUILD_DIR/win

echo "We are building at: $BUILD_DIR"
echo "Our code is located here: $CODE_PATH"

#remove any files in out output dir
rm $OUT_PATH -r -f

#create needed folders
mkdir $OUT_PATH
mkdir $OUT_PATH/sqldrivers
mkdir $OUT_PATH/platforms
mkdir $OUT_PATH/printsupport

#compile the app
cd $OUT_PATH
MINGW=/opt/mingw
export PATH=$PATH:$MINGW/bin
export CFLAGS=""
export CXXFLAGS="$CFLAGS"
cmake $CODE_PATH -DCMAKE_TOOLCHAIN_FILE=$CODE_PATH/packaging/cmake-modules/Win32Toolchain.cmake -DCMAKE_MODULE_PATH=$CODE_PATH/packaging/cmake-modules

#cmake $CODE_PATH -DCMAKE_TOOLCHAIN_FILE=/opt/mxe/usr/i686-pc-mingw32/share/cmake/mxe-conf.cmake
make
