#ifndef __MASTER_STP00_H__
#define __MASTER_STP00_H__

char STP00_set(char LSByte, char Mask, char shift, char Data);
char STP00_get(char NoAdd, char Addr, char Bytes, void *Data_p);
char STP00_put(char NoAdd, char Addr, char Bytes, void *Data_p);

#endif
