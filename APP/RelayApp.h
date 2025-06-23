#ifndef __RelayApp_H
#define __RelayApp_H
#include "sys.h"

/// 10路继电器+2路调光

void RcuData_rev(u8 *dat);
void switch_relay(void);

void Relay_sub(void);

void relay_try(void);

#endif
