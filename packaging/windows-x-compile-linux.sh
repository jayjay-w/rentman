#prepares an environment ready to create windows binaries
CODE_PATH=$1
echo "Our code is located here: $CODE_PATH"

#compile the app
MINGW=/usr
mkdir bin
cp $CODE_PATH/packaging/licence.txt .
cp $CODE_PATH/rentmanager/cottage.ico ./rentman.ico
rm ./rentmanager.nsi
cp $CODE_PATH/rentmanager.nsi .
cp $CODE_PATH/FileAssociation.nsh .
export PATH=$PATH:$MINGW/bin
export CFLAGS=""
export CXXFLAGS="$CFLAGS"
export WINDRES=${CMAKE_SYSTEM_PROCESSOR}-w64-mingw32-windres
export xcmake=${CMAKE_SYSTEM_PROCESSOR}-w64-mingw32-cmake
MODULEPATH=$CODE_PATH/packaging/cmake-modules
i686-w64-mingw32-cmake -DCMAKE_TOOLCHAIN_FILE=$MODULEPATH/Win32Toolchain.cmake -DCMAKE_MODULE_PATH=$MODULEPATH $CODE_PATH
