#ifndef __GT911_H_
#define __GT911_H_

#include <stdint.h>

#define GT_TOUCH_MAX			5			//对于gt911，最多同时获取5个触摸点的数据

typedef enum
{
	X_L = 0,
	X_H = 1,
	Y_L = 2,
	Y_H = 3,
	S_L	= 4,
	S_H = 5
}Data_XYS_P;	//数据X、Y、触摸大小数据偏移量

typedef enum
{
	TOUCH__NO		= 0x00,	//没有触摸
	TOUCH_ING		= 0x80	//被触摸	
}TOUCH_STATE_enum;	//触摸状态

typedef struct
{
	uint16_t	X_Point;	//X坐标
	uint16_t	Y_Point;	//Y坐标
	uint16_t	S_Point;	//触摸点大小
}XY_Coordinate;	//触摸点坐标


/*触摸结构体*/
typedef struct 
{
	uint8_t Touch_State				;	//触摸状态
	uint8_t Touch_Number			;	//触摸数量
	XY_Coordinate Touch_XY[GT_TOUCH_MAX]	;	//触摸的x坐标，对于gt911最多5个点的坐标
}Touch_Struct;	//触摸信息结构体


/*外部变量区*/
extern Touch_Struct	User_Touch;

/*外部函数区*/
void GT911_WriteReg(uint16_t _usRegAddr, uint8_t *_pRegBuf, uint8_t _ucLen);
void GT911_ReadReg(uint16_t _usRegAddr, uint8_t *_pRegBuf, uint8_t _ucLen);

void GT911_Send_Config(uint8_t mode);		//配置初始化ft911
void Software_Reset(uint8_t gt_SR_type);//复位或者不复位gt911
void gt911_Scanf(void);									//扫描触摸模块

#endif /*__GT911_H_*/

