#include "public.h"


#define AUTH_BITS       8


static uchar auth_challenge[AUTH_BITS];
void System_SetAuthChallenge(puchar pChallenge, uchar len)
{
    uchar i;
    if ((!pChallenge)||(len<AUTH_BITS))
        return;
    for(i=0; i<AUTH_BITS; i++) {
        auth_challenge[i] = *(pChallenge+i);
    }
}
uchar System_GetAuthSignature(puchar pSign, uchar len)
{
    uchar i;
    uchar key[AUTH_BITS] = {0xbe, 0xf3, 0xa7, 0x9b, 0x36, 0x2e, 0x41, 0x67};
    
    if ((!pSign)||(len<AUTH_BITS))
        return 0;
    
    for(i=0; i<AUTH_BITS; i++) {
        pSign[i] = (auth_challenge[i]^key[i]);
    }
    for (i=0; i<AUTH_BITS/2; i++) {
        pSign[i] = pSign[2*i]+pSign[2*i+1];
        pSign[AUTH_BITS/2+i] += pSign[i];
    }
    return AUTH_BITS;
}

#define TINY_RAM_SIZE   4
static uchar tiny_ram[TINY_RAM_SIZE]={0};
uchar system_set_tiny_ram(puchar pbuf, uchar len)
{
    uchar i;
    if ((!pbuf)||(len>TINY_RAM_SIZE))
        return 0;
    for (i=0; i<len; i++) {
        tiny_ram[i] = *(pbuf+i);
    }
    return len;
}
uchar system_get_tiny_ram(puchar pbuf, uchar len)
{
    uchar i;
    if ((!pbuf)||(len>TINY_RAM_SIZE))
        return 0;
    for (i=0; i<len; i++) {
        *(pbuf+i) = tiny_ram[i];
    }
    return len;
}
uchar system_get_tiny_ram_size(void)
{
    return TINY_RAM_SIZE;
}


void Clr_CheckSum(void)
{
    CheckHead=0;
    CheckTail=0;
}
bool Check_CheckSum(void)
{
    if(CheckHead!=0x55)
        return FALSE;
    else if(CheckTail!=0xaa)
        return FALSE;
    else 
        return TRUE;
}

void Generate_CheckSum(void)
{
    CheckHead=0x55;
    CheckTail=0xaa;
}


extern void Init_Nochange_Section(void)
{
    FrontSource = NUM_OF_SOURCE;    
//  RearSourceL = NUM_OF_SOURCE;
//  RearSourceR = NUM_OF_SOURCE;
}

