#include "common.h"
typedef uint8_t u8;typedef uint16_t u16;typedef uint32_t u32;typedef uint64_t u64;typedef uint32_t __be32;
#define KERN_INFO ""
#define KERN_ERR ""
#define KERN_WARNING ""
#define printk(...) fprintf(stderr,__VA_ARGS__)
#define pr_info(...) fprintf(stderr,__VA_ARGS__)
#define pr_debug(...) ((void)0)
#define GFP_KERNEL 0
#define kzalloc(n,g) calloc(1,n)
#define kfree free
#define ALIGN(v,a) (((v)+(a)-1)&~((a)-1))
#define min_t(t,a,b) ((t)(a)<(t)(b)?(t)(a):(t)(b))
#define MTD_OPS_PLACE_OOB 0
struct device_node {int unused;};
static int of_property_read_u32(struct device_node*n,const char*p,u32*v){if(!strcmp(p,"mediatek,bmt-max-ratio")){*v=1;return 0;}if(!strcmp(p,"mediatek,bmt-max-reserved-blocks")){*v=64;return 0;}return -EINVAL;}
static bool of_property_read_bool(struct device_node*n,const char*p){return !strcmp(p,"mediatek,empty-page-ecc-protected");}
struct mtd_info {uint64_t size;uint32_t erasesize,oobsize,bitflip_threshold,ecc_strength;};
struct mtd_oob_ops {int mode;size_t ooboffs,ooblen,len,retlen,oobretlen;void *datbuf,*oobbuf;};
struct erase_info {uint64_t addr,len;};
static size_t mtd_oobavail(struct mtd_info*m,struct mtd_oob_ops*o){return m->oobsize;}
#include "linux-types.h"
struct bmt_desc bmtd;
static int read_oob(struct mtd_info*m,loff_t a,struct mtd_oob_ops*o){assert(o->len<=PAGE);int r=read_page_common(a,o->datbuf,o->oobbuf);o->retlen=o->len;o->oobretlen=o->ooblen;return r;}
static int write_oob(struct mtd_info*m,loff_t a,struct mtd_oob_ops*o){assert(o->len<=PAGE);int r=write_page_common(a,o->datbuf,o->oobbuf);o->retlen=o->len;return r;}
static int erase_mtd(struct mtd_info*m,struct erase_info*e){return erase_common(e->addr);}
static int isbad(struct mtd_info*m,loff_t a){return bad[a/BLOCK];}
static int markbad(struct mtd_info*m,loff_t a){bad[a/BLOCK]=1;return 0;}
static bool mtd_is_bitflip(int ret){return ret==-EUCLEAN;}
int bbt_nand_copy(u16 dest_blk, u16 src_blk, loff_t max_offset)
{
	int pages = bmtd.blk_size >> bmtd.pg_shift;
	loff_t src = (loff_t)src_blk << bmtd.blk_shift;
	loff_t dest = (loff_t)dest_blk << bmtd.blk_shift;
	loff_t offset = 0;
	uint8_t oob[64];
	int i, ret;

	for (i = 0; i < pages; i++) {
		struct mtd_oob_ops rd_ops = {
			.mode = MTD_OPS_PLACE_OOB,
			.oobbuf = oob,
			.ooblen = min_t(int, bmtd.mtd->oobsize / pages, sizeof(oob)),
			.datbuf = bmtd.data_buf,
			.len = bmtd.pg_size,
		};
		struct mtd_oob_ops wr_ops = {
			.mode = MTD_OPS_PLACE_OOB,
			.oobbuf = oob,
			.datbuf = bmtd.data_buf,
			.len = bmtd.pg_size,
		};

		if (offset >= max_offset)
			break;

		ret = bmtd._read_oob(bmtd.mtd, src + offset, &rd_ops);
		if (ret < 0 && !mtd_is_bitflip(ret))
			return ret;

		if (!rd_ops.retlen)
			break;

		ret = bmtd._write_oob(bmtd.mtd, dest + offset, &wr_ops);
		if (ret < 0)
			return ret;

		wr_ops.ooblen = rd_ops.oobretlen;
		offset += rd_ops.retlen;
	}

	return 0;
}

bool mapping_block_in_range(int b,int*s,int*e){*s=0;*e=1024;return true;}
