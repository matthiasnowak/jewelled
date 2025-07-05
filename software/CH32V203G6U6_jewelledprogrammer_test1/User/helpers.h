/*
 * helpers.h
 *
 *  Created on: 30 Sep 2024
 *      Author: matth
 */

#ifndef USER_HELPERS_H_
#define USER_HELPERS_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
uint8_t gammaCorrection32(uint8_t gammaval);
uint8_t gammaCorrection64(uint8_t gammaval);
int exampleCEC(void);



#ifdef __cplusplus
}
#endif

#endif /* USER_HELPERS_H_ */
