#!/bin/sh
cd "$(dirname "$0")"
cd ..
make
cd scripts

mkdir ../../tmp
cd ../../tmp
rm -rf *
cp -R ../ofOSCDebugger ofOSCDebugger
zip -r ofOSCDebugger_osx_version.zip ofOSCDebugger
openssl dgst -sha256 ofOSCDebugger_osx_version.zip