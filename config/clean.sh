#!/bin/sh

set -e

SCRIPT=$(readlink -f "$0")
SCRIPTDIR=$(dirname "$SCRIPT")
. $SCRIPTDIR/config.sh

for PROJECT in $PROJECTS; do
    (cd $PROJECT && $MAKE clean)
done

rm -rf build
rm -f Caspian2.iso
