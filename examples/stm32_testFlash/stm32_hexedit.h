/*
 * hexedit.h
 *
 * Created: 18.01.2017 08:24:04
 *  Author: ChrisMicro
 */ 

//
// 修正 2017/03/14, Arduino STM32用に移植, by たま吉さん
// 修正日 2025/01/30 by たま吉さん,Arduino_STM32最新版対応
//

#ifdef __cplusplus
extern "C"
{
	#endif

	#ifndef HEXEDIT_H_
	#define HEXEDIT_H_

	#include "mcurses.h"
	
	void hexedit2 (uint32_t offset, uint8_t flgEdit);

	#endif /* HEXEDIT_H_ */

	#ifdef __cplusplus
}
#endif
