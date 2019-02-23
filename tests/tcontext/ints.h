#ifndef INTS_H
#define INTS_H

#include <stdint.h>

//Указатель для работы с данными из других сегментов
#define FARPTR(seg, off) (((__segment)(seg)):>((uint16_t __based(void)*)(off)))
#define FAR_BYTE_PTR(seg, off) (((__segment)(seg)):>((uint8_t __based(void)*)(off)))

//Изменить адрес обработчика прерывания n на cs:ip
#define SET_INT(n, cs, ip) \
*FARPTR(0, 4*n) = ip;      \
*FARPTR(0, 4*n+2) = cs

//Записать адрес обработчика прерывания n в cs,ip
#define GET_INT(n, cs, ip) \
(ip) = *FARPTR(0, 4*n);    \
(cs) = *FARPTR(0, 4*n+2)

uint16_t GET_CS();
#pragma aux GET_CS = \
"mov ax, cs"         \
value [ax];

#define NEXT_SEG(s) (s + 0x1000)

#endif
