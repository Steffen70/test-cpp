#!/usr/bin/env bash
set -euo pipefail

# Dependencies:
#   sudo apt install protobuf-c-compiler libprotobuf-c-dev

DIR="$(cd "$(dirname "${BASH_SOURCE}")" && pwd)"
PROTO_ROOT="$DIR/../protos"
OUTDIR="$DIR/generated"

# Clean output root
rm -rf "$OUTDIR"
mkdir -p "$OUTDIR"

# Collect all .proto files under PROTO_ROOT (recursively)
mapfile -d '' PROTO_FILES < <(find "$PROTO_ROOT" -type f -name '*.proto' -print0)

echo "Generating C for ${#PROTO_FILES[@]} .proto files into $OUTDIR"
protoc \
    --plugin=protoc-gen-c="$(command -v protoc-gen-c)" \
    --proto_path="$PROTO_ROOT" \
    --c_out="$OUTDIR" \
    "${PROTO_FILES[@]}"

echo "All C protobufs generated under $OUTDIR"
