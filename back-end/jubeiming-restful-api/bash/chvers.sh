BIN_DIRECTORY=/bin

if [ "$1" == "gcc" ] || [ "$1" == "g++" ]; then
    if [ "$2" == "11" ] || [ "$2" == "12" ]; then
        rm $BIN_DIRECTORY/gcc
        rm $BIN_DIRECTORY/g++
        ln -s $BIN_DIRECTORY/gcc-$2 $BIN_DIRECTORY/gcc
        ln -s $BIN_DIRECTORY/g++-$2 $BIN_DIRECTORY/g++
        echo "The current version of gcc and g++ is $2"
    else
        echo "Invalid input"
    fi
else
    echo "Invalid input"
fi