#!/bin/sh
set -eu
cd "$(dirname "$0")/../.."
export SOURCE_DATE_EPOCH=1775529371
cp contrib/n30pro/feeds.conf feeds.conf
./scripts/feeds update -a
./scripts/feeds install -a
python3 contrib/n30pro/fetch-mihomo.py
cp contrib/n30pro/full.config .config
make defconfig
make download -j4
make -j"${N30_BUILD_JOBS:-4}" V=s
