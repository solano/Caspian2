#!/bin/sh

set -e
SCRIPT=$(readlink -f "$0")
SCRIPTDIR=$(dirname "$SCRIPT")
. $SCRIPTDIR/config.sh

mkdir -p "$SYSROOT"

for PROJECT in $PROJECTS; do
    (cd $PROJECT && DESTDIR="$SYSROOT" $MAKE install)
done
