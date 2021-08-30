#!/bin/bash
rm -rf ./build/**
cd ./edk2/ && source ./edksetup.sh && build
cp /home/vscode/edk2/Build/MosLoaderPkgX64/DEBUG_CLANG38/X64/Loader.efi /home/vscode/mos/build/Loader.efi
cd /home/vscode/mos/build/ && run_qemu.sh Loader.efi