SYSTEM_HEADER_PROJECTS="kernel"
PROJECTS="kernel"

export MAKE=${MAKE:-make}
export HOST=${HOST:-i586-elf}

export CC=${HOST}-gcc
export AR=${HOST}-ar
export LD=${HOST}-ld
export AS=nasm

export BUILD_DIR="$(pwd)/build"
export SYSROOT="$BUILD_DIR/sysroot"

# these will be inside sysroot
export BOOTDIR=/boot
export LIBDIR=/lib
export INCLUDEDIR=/include

# Cross-compiler: I command you to use my sysroot!
export SYSROOT="$BUILD_DIR/sysroot"
export CC="$CC --sysroot=$SYSROOT"

# Also, my include directory.
if echo "$HOST" | grep -Eq -- '-elf($|-)'; then
    export CC="$CC -isystem=$INCLUDEDIR"
fi
