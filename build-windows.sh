#!/bin/bash
rm -r windows
INCLUDES='-Isrc -Igtk+-mingw/include -Igtk+-mingw/lib/glib-2.0/include -I/usr/include/c++/6/'
for include_folder in $(ls -d gtk+-mingw/include/*/); do
    INCLUDES="$INCLUDES -I$include_folder"
done
LIB='-Lgtk+-mingw/lib'
LINK="$(pkg-config --libs gtk+-3.0) -lfreetype -std=c++14 -lstdc++ -lm"
CC=i686-w64-mingw32-g++

for source in $(find src -name '*.cpp'); do
    echo "Compiling $source..."
    mkdir -p windows/$(dirname $source)
    echo "$CC -std=c++14 -lm $source $INCLUDES -c -o windows/$source.o"
    $CC -std=c++14 -lm -lstdc++fs $source $INCLUDES -c -o windows/$source.o
done


mkdir windows/bin
OBJ=$(find windows/ -name '*.o' | tr '\n' ' ')
echo "$CC $OBJ $INCLUDES $LIB $LINK -o windows/bin/GraphColoring.exe"
$CC $OBJ $INCLUDES $LIB $LINK -o windows/bin/GraphColoring.exe

echo 'Copying files...'
rm -r windows/src
mv windows/bin/GraphColoring.exe windows/
rmdir windows/bin

cp /usr/lib/gcc/i686-w64-mingw32/6.3-win32/libgcc_s_sjlj-1.dll windows/
cp /usr/lib/gcc/i686-w64-mingw32/6.3-win32/libstdc++-6.dll windows/
cp gtk+-mingw/bin/*.dll windows/
cp -r assets windows/
echo 'Done!'
