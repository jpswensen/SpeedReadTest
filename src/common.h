#ifndef __COMMON_H__
#define __COMMON_H__

#define DEBUG_SIMPLE 1
#define DEBUG_LOCATE 2

#define DEBUG DEBUG_SIMPLE

#if defined(DEBUG) && DEBUG == 1
 #define DEBUG_PRINT(fmt, args...) printf(fmt, ##args)
#elif defined(DEBUG) && DEBUG == 2
 #define DEBUG_PRINT(fmt, args...) printf("DEBUG: %s:%d:%s(): " fmt, \
    __FILE__, __LINE__, __func__, ##args)
#else
 #define DEBUG_PRINT(fmt, args...) /* Don't do anything in release builds */
#endif

int led = 13;

typedef enum ModeEnum {
  INITIALIZING = 1,  
  WAITING = 2,
  ARMED = 3,
  RESPOND_COMMAND = 4,
  FIRING = 5
} Mode_t;

typedef enum ErrorEnum {
  NONE_ERROR = 0,
  BEAM_ERROR = 1,
  MODE_ERROR = 2,
  CAPTURE_ERROR = 3,
  COMM_ERROR = 4
} Error_t;



#endif // __COMMON_H__
