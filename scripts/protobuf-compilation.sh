#!/usr/bin/env bash

mkdir -p common/include/r-type/proto
protoc -I=common --cpp_out=common/include/r-type/proto common/packets.proto
