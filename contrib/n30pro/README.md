# Netcore N30 Pro: historical r3 experimental firmware

This branch contains the source configuration used for the r3 image built and tested on 2026-10-14. It is independent of the Linux 6.18 NMBM proposal associated with openwrt/openwrt#24316. The official project and the PR author have not endorsed this experimental build.

## Compatibility and known limits

The binary requires an already installed NMBM-enabled web U-Boot, valid existing NMBM v1 tables and the N30 Pro layout below. It is not an installation image for stock/raw NAND. Its historical board ID is `netcore,n30pro`, compatibility 2.0; the new proposal uses the different ID `netcore,n30-pro-nmbm`. Do not force-upgrade between these profiles. No migration procedure between them has been validated. No bootloader or calibration backups are distributed.

Physical flash was ESMT F50L1G41LB, 128 MiB; NMBM exposes 120 MiB with a 64-block reserve limit. Logical partitions: BL2 0x0+0x100000, u-boot-env 0x100000+0x80000, Factory 0x180000+0x200000, FIP 0x380000+0x200000, ubi 0x580000+0x7280000. Other NAND parts, NX35U and machines with existing bad blocks are untested. A zero bad-block count on one unit is not a guarantee that a firmware has no bad blocks.

The tested unit still reported a UBI reserve warning: 17 reserved PEBs vs 19 expected; bad and corrupted PEBs were 0. This is unresolved and is why the release remains experimental. NMBM may repair a single metadata table, so loading a map is not guaranteed to be write-free. Automatic map creation is disabled in Linux. The selected production U-Boot enables CREATE; the simulation's no-create rejection tests do not imply otherwise.

## Build

Use a Linux OpenWrt build environment (the original was Ubuntu ARM64; the container digest is recorded in source-lock.json). Install the normal OpenWrt build prerequisites, then run `sh contrib/n30pro/build.sh`. The script locks all feed revisions, fetches the original official Mihomo ARM64 ELF and checks both compressed and ELF hashes, restores the complete r3 config and builds. Downloads require network access. The emitted image is under bin/targets/mediatek/filogic.

The complete OpenWrt sources, patches and local OpenClash package payload are in this branch. GPL source archives for Mihomo v1.19.32 and OpenClash v0.48.002 accompany the release; they include upstream build scripts and dependency manifests. For a modified Mihomo, build that source with `make linux-arm64 VERSION=v1.19.32` (upstream uses CGO_ENABLED=0, with_gvisor, trimpath and version/build-time ldflags), then replace package/local/mihomo/files/clash_meta. The default recipe intentionally restores the original hashed upstream binary. Upstream release tooling used a MetaCubeX Go 1.26 toolchain; rebuilding with different toolchains or timestamps can change the ELF hash.

Only code not selected or included in the r3 sysupgrade image was excluded for privacy: the per-device recovery package/binding and unused recovery DTS. All retained patched runtime files and local payloads were checked against the original source lock. The inactive generic recovery kernel hook is retained because it was compiled into the r3 kernel. Private image/package signing keys are excluded; builds generate their own keys. Consequently, this is corresponding source and a build recipe, not a claim of a bit-for-bit reproduced signed firmware. Version files preserve the historical build label.

OpenClash 0.48.002 and Mihomo 1.19.32 are bundled. The original image has proxy and automatic updates disabled, with no personal subscription/node configuration. Fresh default OpenWrt credentials must be configured by the installer. Later device settings, firewall rules, passwords and subscriptions are not part of this release.

## Evidence

`validation/` contains sanitized checks. The r3 image was hardware-tested on one N30 Pro, with FIT readback matching the published SHA256 and BL2/Factory/FIP unchanged. LAN4 negotiated 1 Gbps and 2.4/5 GHz association and LuCI access passed. LAN1-3 were previously tested under r2 and were not replugged during r3 verification. Internet/proxy functionality was not part of the original r3 acceptance; later live configuration is not distributed. The Linux 6.18 source checks and synthetic mapping tests are labeled separately and do not establish hardware acceptance of that proposal.

Original OpenWrt sources retain their licenses. OpenClash and Mihomo retain GPL-3.0 licensing. See the source trees and accompanying archives for license texts and upstream authors. The NMBM harness preserves MediaTek attribution and uses public code with synthetic NAND fixtures only.
