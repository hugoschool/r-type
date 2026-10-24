#!/usr/bin/env bash

# TODO: add a check
echo "This script has to be ran from root of the repository."

if [ "$#" -ne 2 ]; then
    echo "Usage:"
    echo "  ./bump-version.sh <OLD_VERSION> <NEW_VERSION>"
    exit 1
fi

OLD_VERSION=$1
NEW_VERSION=$2

sed -i "s/set(version $OLD_VERSION)/set(version $NEW_VERSION)/" CMakeLists.txt
sed -i "s/set(version $OLD_VERSION)/set(version $NEW_VERSION)/" engine/CMakeLists.txt

echo "Versions have been bumped from $OLD_VERSION to $NEW_VERSION"
