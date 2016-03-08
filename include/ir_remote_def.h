/*
 * ir_remote_def.h
 *
 * Version 1.1, 08.03.2016
 * Written by Valeriy Kucherenko
 * For details, see https://github.com/valkuc/esp8266-ir-remote
 *
 * IR code defines are based on https://github.com/shirriff/Arduino-IRremote by Ken Shirriff
 */

#ifndef __IR_REMOTE_DEF_H__
#define __IR_REMOTE_DEF_H__

#define TOPBIT 0x80000000

#define NEC_FREQUENCY	38400
#define NEC_HDR_MARK	9000
#define NEC_HDR_SPACE	4500
#define NEC_BIT_MARK	562
#define NEC_ONE_SPACE	1687
#define NEC_ZERO_SPACE	562

#define PANASONIC_FREQUENCY		35000
#define PANASONIC_HDR_MARK 		3502
#define PANASONIC_HDR_SPACE 	1750
#define PANASONIC_BIT_MARK 		502
#define PANASONIC_ONE_SPACE 	1244
#define PANASONIC_ZERO_SPACE 	400

#define SONY_FREQUENCY	40000
#define SONY_HDR_MARK	2400
#define SONY_HDR_SPACE	600
#define SONY_ONE_MARK	1200
#define SONY_ZERO_MARK	600

#define SAMSUNG_FREQUENCY	38400
#define SAMSUNG_HDR_MARK  	5000
#define SAMSUNG_HDR_SPACE 	5000
#define SAMSUNG_BIT_MARK  	560
#define SAMSUNG_ONE_SPACE 	1600
#define SAMSUNG_ZERO_SPACE  560

#define RC5_FREQUENCY		36000
#define RC5_T1				889

#define RC6_FREQUENCY		36000
#define RC6_HDR_MARK		2666
#define RC6_HDR_SPACE		889
#define RC6_T1				444

#endif
