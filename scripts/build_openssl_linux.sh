#!/bin/bash
set -e

DEST_DIR="/mnt/d/Personal/Projects/C++/dotnetdupe/external/openssl-4.0.1/linux-x64"
SRC_DIR="/home/sudhe/openssl-4.0.1-src"

echo "=== Cleaning previous build directories ==="
rm -rf "$SRC_DIR"
mkdir -p "$DEST_DIR"

echo "=== Cloning OpenSSL openssl-4.0.1 tag ==="
git clone --depth 1 --branch openssl-4.0.1 https://github.com/openssl/openssl.git "$SRC_DIR"

echo "=== Configuring OpenSSL 4.0.1 for linux-x64 ==="
cd "$SRC_DIR"
./config shared -Wl,-rpath,'$ORIGIN' --prefix="$DEST_DIR" --openssldir="$DEST_DIR/ssl"

echo "=== Compiling OpenSSL 4.0.1 ==="
make -j$(nproc)

echo "=== Installing OpenSSL 4.0.1 into $DEST_DIR ==="
make install_sw

echo "=== Copying static libraries to x64/lib/linux ==="
mkdir -p /mnt/d/Personal/Projects/C++/dotnetdupe/external/openssl-4.0.1/x64/lib/linux
if [ -d "$DEST_DIR/lib64" ]; then
    cp "$DEST_DIR"/lib64/*.a /mnt/d/Personal/Projects/C++/dotnetdupe/external/openssl-4.0.1/x64/lib/linux/ || true
elif [ -d "$DEST_DIR/lib" ]; then
    cp "$DEST_DIR"/lib/*.a /mnt/d/Personal/Projects/C++/dotnetdupe/external/openssl-4.0.1/x64/lib/linux/ || true
fi

echo "=== OpenSSL 4.0.1 Linux Build Complete ==="
ls -la "$DEST_DIR"
