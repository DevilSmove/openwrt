#include "common.h"
#include "uboot-core.c"
static int rd(void*a,uint64_t x,void*d,void*o,enum nmbm_oob_mode mode){return read_page_common(x,d,o);}
static int wr(void*a,uint64_t x,const void*d,const void*o,enum nmbm_oob_mode mode){return write_page_common(x,d,o);}
static int er(void*a,uint64_t x){return erase_common(x);}
static int ib(void*a,uint64_t x){return bad[x/BLOCK];}
static int mb(void*a,uint64_t x){bad[x/BLOCK]=1;return 0;}
int main(int argc,char**argv){assert(argc==4);load_flash(argv[1]);struct nmbm_lower_device dev={.max_ratio=1,.max_reserved_blocks=64,.flags=NMBM_F_EMPTY_PAGE_ECC_OK | (!strcmp(argv[3],"no-create") ? 0 : NMBM_F_CREATE),.size=SIZE,.erasesize=BLOCK,.writesize=PAGE,.oobsize=64,.oobavail=64,.read_page=rd,.write_page=wr,.erase_block=er,.is_bad_block=ib,.mark_bad_block=mb};struct nmbm_instance*ni=calloc(1,nmbm_calc_structure_size(&dev));int ret=nmbm_attach(&dev,ni);
 if(!ret&&!strcmp(argv[3],"remap")){erase_fail=51;assert(!nmbm_erase_block_range(ni,51u*BLOCK,BLOCK,NULL));assert(ni->block_mapping[51]!=51);}
 printf("{\"ret\":%d,\"size\":%llu,\"writes\":%u,\"erases\":%u",ret,(unsigned long long)(ret?SIZE:nmbm_get_avail_size(ni)),writes,erases);
 if(!ret){printf(",\"map\":[");for(int i=0;i<960;i++)printf("%s%d",i?",":"",ni->block_mapping[i]);unsigned char data[PAGE];assert(!nmbm_read_single_page(ni,50u*BLOCK,data,NULL,NMBM_MODE_PLACE_OOB));printf("],\"data_crc\":%u",crc32_le(0,data,PAGE));}
 puts("}");save_flash(argv[2]);free(ni);free(flash);return 0;}
