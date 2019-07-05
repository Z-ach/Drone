DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"
cmake -S $DIR/../src -B $DIR
make .
mkdir -p $DIR/../bin
mv $DIR/Drone $DIR/../bin/
