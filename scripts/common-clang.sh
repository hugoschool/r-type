BASE_DIR=$(realpath $(cd $(dirname "$0")/.. && pwd))

CLIENT_FILES=$(find $BASE_DIR/client/ -type f -iname "*.cpp" -o -iname "*.hpp")
COMMON_FILES=$(find $BASE_DIR/common/ -type f -iname "*.cpp" -o -iname "*.hpp")
ENGINE_FILES=$(find $BASE_DIR/engine/ -type f -iname "*.cpp" -o -iname "*.hpp")
SERVER_FILES=$(find $BASE_DIR/server/ -type f -iname "*.cpp" -o -iname "*.hpp")
NETWORK_FILES=$(find $BASE_DIR/network/ -type f -iname "*.cpp" -o -iname "*.hpp")

FILES=(
    $CLIENT_FILES
    $COMMON_FILES
    $ENGINE_FILES
    $SERVER_FILES
    $NETWORK_FILES
)
