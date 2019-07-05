DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"
cd $DIR
rm -r CMake*
rm cmake*
rm Makefile
cmake ../src
make
mkdir -p ../bin
mv Drone ../bin
