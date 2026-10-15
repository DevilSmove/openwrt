#include <assert.h>
#include <stdio.h>
#include <string.h>
static int fail, declared, registered, cleaned;
static int mtk_bmt_attach(void *p) { return fail; }
static void *mtd_get_of_node(void *p) { return p; }
static int of_property_read_bool(void *p, const char *s) {
    assert(!strcmp(s, "mediatek,nmbm")); return declared;
}
#define dev_err(...) ((void)0)
static int mtd_device_register(void *p, void *q, int n) { registered++; return 0; }
static int probe(void) { int ret; void *mtd = NULL;
	ret = mtk_bmt_attach(mtd);
	if (ret && of_property_read_bool(mtd_get_of_node(mtd),
					 "mediatek,nmbm")) {
		dev_err(&mem->spi->dev,
			"Required NMBM attach failed (%d); refusing raw MTD fallback\n",
			ret);
		goto err_spinand_cleanup;
	}

	ret = mtd_device_register(mtd, NULL, 0);
    return ret;
err_spinand_cleanup:
    cleaned++; return ret;
}
int main(void) {
    fail = -19; declared = 1; assert(probe() == -19 && !registered && cleaned == 1);
    fail = -22; assert(probe() == -22 && !registered && cleaned == 2);
    fail = 0; assert(!probe() && registered == 1 && cleaned == 2);
    declared = 0; fail = -19; assert(!probe() && registered == 2 && cleaned == 2);
    puts("PASS: four exact-fragment probe guard cases");
}
