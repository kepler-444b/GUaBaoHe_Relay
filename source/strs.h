#ifndef __STRS_H
#define __STRS_H	 
#include "sys.h"



/*******************************************************************************
 **清除缓存数据
 **支持结构体和数组
 ******************************************************************************/
#define Bufclr(x)          Strbufclr((uint8_t*)&(x), (uint32_t)(sizeof(x)))

void Strbufclr(void *pu8Address, u32 u32Count);
void StrToHexs(u8 *BufIn,u8 *Outs,u8 len);








#endif








