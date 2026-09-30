BASE_DIR=$(realpath $(dirname "$0"))

FILES=(
    $(find $BASE_DIR/../client/ -type f -iname "*.cpp" -o -iname "*.hpp")
    $(find $BASE_DIR/../engine/ -type f -iname "*.cpp" -o -iname "*.hpp")
    $(find $BASE_DIR/../server/ -type f -iname "*.cpp" -o -iname "*.hpp")
)
