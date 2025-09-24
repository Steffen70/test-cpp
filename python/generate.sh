#!/usr/bin/env bash
set -e

DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
OUTDIR="$DIR/generated"

# Create or clean the output directory
if [ -d "$OUTDIR" ]; then
    rm -rf "$OUTDIR"/*
else
    mkdir -p "$OUTDIR"
fi

# Generate Python gRPC stubs
pipenv run python -m grpc_tools.protoc \
  -I "$DIR/../protos" \
  --python_out="$OUTDIR" \
  --pyi_out=generated \
  "$DIR/../protos/student.proto"

touch generated/__init__.py
