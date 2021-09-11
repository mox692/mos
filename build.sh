#!/bin/bash

rm -rf ./build/**
rm -rf ./edk2/Build

source $HOME/mos/osbook/devenv/buildenv.sh
# cd ./kernel/ && ./build.sh && cd ..
cd ./kernel && make clean && make && cd ../

# cd $HOME/edk2/ && source ./edksetup.sh && build
# cp $HOME/edk2/Build/MosLoaderPkgX64/DEBUG_CLANG38/X64/Loader.efi $HOME/mos/build/Loader.efi
# cd $HOME/mos/build/ && run_qemu.sh $HOME/mos/build/Loader.efi $HOME/mos/kernel/kernel.elf
