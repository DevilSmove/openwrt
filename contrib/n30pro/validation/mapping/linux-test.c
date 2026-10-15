#include "linux-shim.h"
#include "linux-core.c"
int main(int argc,char**argv){assert(argc==4);load_flash(argv[1]);struct mtd_info m={.size=SIZE,.erasesize=BLOCK,.oobsize=64,.bitflip_threshold=6,.ecc_strength=8};
 bmtd=(struct bmt_desc){.mtd=&m,.pg_size=PAGE,.blk_size=BLOCK,.pg_shift=11,.blk_shift=17,.total_blks=1024,.data_buf=calloc(1,PAGE+64),._read_oob=read_oob,._write_oob=write_oob,._erase=erase_mtd,._block_isbad=isbad,._block_markbad=markbad};
 struct device_node np;int ret=mtk_bmt_init_nmbm(&np);if(!ret&&!strcmp(argv[3],"remap")){int p=get_mapping_block_index_nmbm(50);assert(remap_block_nmbm(50,p,BLOCK));}
 printf("{\"ret\":%d,\"size\":%llu,\"writes\":%u,\"erases\":%u",ret,(unsigned long long)m.size,writes,erases);
 if(!ret){printf(",\"map\":[");for(int i=0;i<960;i++)printf("%s%d",i?",":"",get_mapping_block_index_nmbm(i));printf("],\"data_crc\":%u",crc32_le(0,flash+(size_t)get_mapping_block_index_nmbm(50)*BLOCK,PAGE));}
 puts("}");save_flash(argv[2]);free(bmtd.ni);free(bmtd.data_buf);free(flash);return 0;}
