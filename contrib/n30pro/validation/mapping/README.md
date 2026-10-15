# Synthetic NMBM interoperability checks

Linux algorithm: target/linux/generic/files/drivers/mtd/nand/mtk_bmt_nmbm.c at OpenWrt PR #24316 head 39f06f46e94015a0d9c23e548b8e7b09135277b9. The algorithm from NMBM_MAGIC_SIGNATURE through EOF is unchanged; only host callbacks/types replace the kernel environment. U-Boot source URLs and hashes are in uboot-source-lock.json. No physical device dump or identity is used.

Run in this directory, with Clang and Python 3:

```sh
cc -O1 -g -fsanitize=address,undefined -Wno-format -I. linux-test.c -o linux-test
cc -O1 -g -fsanitize=address,undefined -Wno-format -I. uboot-test.c -o uboot-test
python3 run-tests.py
```

About 1.5 GiB scratch space is needed. Fixtures are generated locally and are not published. macOS LeakSanitizer is disabled; AddressSanitizer and UndefinedBehaviorSanitizer are enabled. The U-Boot CREATE flag is enabled for positive cases; negative comparisons explicitly disable it. These host simulations do not replace hardware/ECC/power-loss testing. A single-table recovery writes metadata and is reported as such.
