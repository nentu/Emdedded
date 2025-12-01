/*
 * utils.h
 *
 *  Created on: Dec 1, 2025
 *      Author: zam12
 */

#ifndef INC_UTILS_H_
#define INC_UTILS_H_


 #define max(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a > _b ? _a : _b; })

 #define min(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a < _b ? _a : _b; })

#endif /* INC_UTILS_H_ */
