#!/bin/bash -e

VERSION=`grep -w RG_VERSION $JMK_ROOT/pkg/include/rg_version_data.h | \
  awk '{print $3}' | sed "s/\"//g"`
ARCH=i386
[ "$(file -b $(which file) | grep -o 64-bit)" == "64-bit" ] && ARCH=amd64

PACKAGES=`find $CRAMFS_DIR -maxdepth 2 -type d -name "DEBIAN" | xargs -n 1 dirname`

for dir in $PACKAGES; do
  deps=$(echo JMK_PACAKGE_$(basename $dir)_DEPENDS | sed "s/-/_/g")
  sed -i -e "s/_VERSION_/$VERSION/g" -e "s/_ARCH_/$ARCH/g" \
    -e "s/_DEPS_/${!deps}/g" -e "s/: ,/:/g" -e "/Depends: $/d" \
    -e "s/_SIZE_/`du -s $dir | awk '{print \$1}'`/g" $dir/DEBIAN/control
done
