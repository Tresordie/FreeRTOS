/**
  ******************************************************************************
  * @file    usb_core.h
  * @author  MCD Application Team
  * @version V4.1.0
  * @date    26-May-2017
  * @brief   Standard protocol processing functions prototypes
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2017 STMicroelectronics</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/**
  ******************************************************************************
  * usb_core(.h, .c)的作用
  *
  * 1. USB全速设备库的内核，其实现了USB2.0规范第9章中的所有功能
  * 2. 其可用的子程序可以处理与控制端点(Endpoint0)相关的USB标准请求(USB解包，USB封包)
  *    并提供必要的代码以实现顺序枚举
  * 3. USB_code模块实现了一个标准请求和用户使用结构体User_Standard_Requests实现之间的动态接口
  * 4. USB_core在必要时，发送类特殊请求个一些总线事件到用户程序，Device_Property结构体中给出了
  *    用户处理进程(代码)
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USB_CORE_H
#define __USB_CORE_H


/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
typedef enum _CONTROL_STATE
{
  WAIT_SETUP,       /* 0 */
  SETTING_UP,       /* 1 */
  IN_DATA,          /* 2 */
  OUT_DATA,         /* 3 */
  LAST_IN_DATA,     /* 4 */
  LAST_OUT_DATA,    /* 5 */
  WAIT_STATUS_IN,   /* 7 */
  WAIT_STATUS_OUT,  /* 8 */
  STALLED,          /* 9 */
  PAUSE             /* 10 */
} CONTROL_STATE;    /* The state machine states of a control pipe */

typedef struct OneDescriptor
{
  uint8_t *Descriptor;
  uint16_t Descriptor_Size;
}
ONE_DESCRIPTOR, *PONE_DESCRIPTOR;
/* All the request process routines return a value of this type
   If the return value is not SUCCESS or NOT_READY,
   the software will STALL the correspond endpoint */
typedef enum _RESULT
{
  USB_SUCCESS = 0,    /* Process successfully */
  USB_ERROR,
  USB_UNSUPPORT,
  USB_NOT_READY       /* The process has not been finished, endpoint will be
                         NAK to further request */
} RESULT;


/*-*-*-*-*-*-*-*-*-*-* Definitions for endpoint level -*-*-*-*-*-*-*-*-*-*-*-*/
typedef struct _ENDPOINT_INFO
{
  /* When send data out of the device,
   CopyData() is used to get data buffer 'Length' bytes data
   if Length is 0,
    CopyData() returns the total length of the data
    if the request is not supported, returns 0
    (NEW Feature )
     if CopyData() returns -1, the calling routine should not proceed
     further and will resume the SETUP process by the class device
   if Length is not 0,
    CopyData() returns a pointer to indicate the data location
   Usb_wLength is the data remain to be sent,
   Usb_wOffset is the Offset of original data
  When receive data from the host,
   CopyData() is used to get user data buffer which is capable
   of Length bytes data to copy data from the endpoint buffer.
   if Length is 0,
    CopyData() returns the available data length,
   if Length is not 0,
    CopyData() returns user buffer address
   Usb_rLength is the data remain to be received,
   Usb_rPointer is the Offset of data buffer
  */
  uint16_t  Usb_wLength;
  uint16_t  Usb_wOffset;
  uint16_t  PacketSize;
  uint8_t   *(*CopyData)(uint16_t Length);
}ENDPOINT_INFO;

/*-*-*-*-*-*-*-*-*-*-*-* Definitions for device level -*-*-*-*-*-*-*-*-*-*-*-*/

/* 设备表结构体Device_Table：设备底层信息保存在设备表结构体中 */
typedef struct _DEVICE
{
  uint8_t Total_Endpoint;     /* Number of endpoints that are used */
  uint8_t Total_Configuration;/* Number of configuration available */
}
DEVICE;

typedef union
{
  uint16_t w;
  struct BW
  {
    uint8_t bb1;
    uint8_t bb0;
  }
  bw;
} uint16_t_uint8_t;



/**
  * USB_core为Device_Info结构体中应用的USB设备保存来自USB主机建立包
  */
typedef struct _DEVICE_INFO
{
  uint8_t USBbmRequestType;             /* bmRequestType -> 一个建立包中bmRequestType的副本*/
  uint8_t USBbRequest;                  /* bRequest -> 一个建立包中bRequest的副本 */
  uint16_t_uint8_t USBwValues;          /* wValue -> 一个建立包中wValue的副本 */
  uint16_t_uint8_t USBwIndexs;          /* wIndex -> 一个建立包中wIndex的副本 */
  uint16_t_uint8_t USBwLengths;         /* wLength -> 一个建立包中wLength的副本 */

  uint8_t ControlState;                 /* of type CONTROL_STATE -> 状态机的控制状态，指明当前USB所处的状态
                                            (等待建立，正在建立，数据输入，数据输出，最近输入，最近输出) */
  uint8_t Current_Feature;              /* 任何时间的设备特征，其受SET_FEATURE及CLEAR_FEATURE请求影响且可以
                                           通过GET_STATUS请求重新获取，用户代码不会用到此域 */
  uint8_t Current_Configuration;        /* 设备在任何时间的工作配置，可以通过SET_CONFIGURATION请求来设置，用
                                           GET_CONFIGURATION请求来获取 */
  uint8_t Current_Interface;            /* Selected interface of current configuration -> 当前被选定的接口 */
  uint8_t Current_AlternateSetting;     /* Selected Alternate Setting of current interface -> 当前选定的工作配置及接口的可选择型*/

  ENDPOINT_INFO Ctrl_Info;
}DEVICE_INFO;


/**
  * USB_core在任何时候都可以给user的代码传送控制，
  * Device_Property中为对USB_core控制的相应处理程序
  */
typedef struct _DEVICE_PROP
{
  void (*Init)(void);        /* Initialize the device */
  void (*Reset)(void);       /* Reset routine of this device */

  /* Device dependent process after the status stage */
  void (*Process_Status_IN)(void);
  void (*Process_Status_OUT)(void);

  /* Procedure of process on setup stage of a class specified request with data stage */
  /* All class specified requests with data stage are processed in Class_Data_Setup
   Class_Data_Setup()
    responses to check all special requests and fills ENDPOINT_INFO
    according to the request
    If IN tokens are expected, then wLength & wOffset will be filled
    with the total transferring bytes and the starting position
    If OUT tokens are expected, then rLength & rOffset will be filled
    with the total expected bytes and the starting position in the buffer

    If the request is valid, Class_Data_Setup returns SUCCESS, else UNSUPPORT

   CAUTION:
    Since GET_CONFIGURATION & GET_INTERFACE are highly related to
    the individual classes, they will be checked and processed here.
  */
  RESULT (*Class_Data_Setup)(uint8_t RequestNo);

  /* Procedure of process on setup stage of a class specified request without data stage */
  /* All class specified requests without data stage are processed in Class_NoData_Setup
   Class_NoData_Setup
    responses to check all special requests and perform the request

   CAUTION:
    Since SET_CONFIGURATION & SET_INTERFACE are highly related to
    the individual classes, they will be checked and processed here.
  */
  RESULT (*Class_NoData_Setup)(uint8_t RequestNo);

  /*Class_Get_Interface_Setting
   This function is used by the file usb_core.c to test if the selected Interface
   and Alternate Setting (uint8_t Interface, uint8_t AlternateSetting) are supported by
   the application.
   This function is writing by user. It should return "SUCCESS" if the Interface
   and Alternate Setting are supported by the application or "UNSUPPORT" if they
   are not supported. */

  RESULT  (*Class_Get_Interface_Setting)(uint8_t Interface, uint8_t AlternateSetting);

  uint8_t* (*GetDeviceDescriptor)(uint16_t Length);
#ifdef LPM_ENABLED
  uint8_t* (*GetBosDescriptor)(uint16_t Length);
#endif
  uint8_t* (*GetConfigDescriptor)(uint16_t Length);
  uint8_t* (*GetStringDescriptor)(uint16_t Length);

  /* This field is not used in current library version. It is kept only for 
   compatibility with previous versions */
  void* RxEP_buffer;
   
  uint8_t MaxPacketSize;

}DEVICE_PROP;


/**
  * USER_STANDARD_REQUESTS是标准请求管理与用户代码user code
  * 之间的接口.
  * 1. 如果用户在接收到一个标准的USB设备请求之后，它需要通过
  *    使用当前结构体中响应的函数来执行特定的代码.
  * 2. 为了实现类请求及应用特定控制，应用开发这必须执行这3个结构体
  *    DEVICE_PROP,Device_Table及USER_STANDARD_REQUEST
  */
typedef struct _USER_STANDARD_REQUESTS
{
  void (*User_GetConfiguration)(void);       /* Get Configuration */
  void (*User_SetConfiguration)(void);       /* Set Configuration */
  void (*User_GetInterface)(void);           /* Get Interface */
  void (*User_SetInterface)(void);           /* Set Interface */
  void (*User_GetStatus)(void);              /* Get Status */
  void (*User_ClearFeature)(void);           /* Clear Feature */
  void (*User_SetEndPointFeature)(void);     /* Set Endpoint Feature */
  void (*User_SetDeviceFeature)(void);       /* Set Device Feature */
  void (*User_SetDeviceAddress)(void);       /* Set Device Address */
}
USER_STANDARD_REQUESTS;

/* Exported constants --------------------------------------------------------*/
#define Type_Recipient (pInformation->USBbmRequestType & (REQUEST_TYPE | RECIPIENT))

#define Usb_rLength Usb_wLength
#define Usb_rOffset Usb_wOffset

#define USBwValue USBwValues.w
#define USBwValue0 USBwValues.bw.bb0
#define USBwValue1 USBwValues.bw.bb1
#define USBwIndex USBwIndexs.w
#define USBwIndex0 USBwIndexs.bw.bb0
#define USBwIndex1 USBwIndexs.bw.bb1
#define USBwLength USBwLengths.w
#define USBwLength0 USBwLengths.bw.bb0
#define USBwLength1 USBwLengths.bw.bb1

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
uint8_t Setup0_Process(void);
uint8_t Post0_Process(void);
uint8_t Out0_Process(void);
uint8_t In0_Process(void);

RESULT Standard_SetEndPointFeature(void);
RESULT Standard_SetDeviceFeature(void);

uint8_t *Standard_GetConfiguration(uint16_t Length);
RESULT Standard_SetConfiguration(void);
uint8_t *Standard_GetInterface(uint16_t Length);
RESULT Standard_SetInterface(void);
uint8_t *Standard_GetDescriptorData(uint16_t Length, PONE_DESCRIPTOR pDesc);

uint8_t *Standard_GetStatus(uint16_t Length);
RESULT Standard_ClearFeature(void);
void SetDeviceAddress(uint8_t);
void NOP_Process(void);

extern DEVICE_PROP Device_Property;
extern  USER_STANDARD_REQUESTS User_Standard_Requests;
extern  DEVICE  Device_Table;
extern DEVICE_INFO Device_Info;

/* cells saving status during interrupt servicing */
extern __IO uint16_t SaveRState;
extern __IO uint16_t SaveTState;

#endif /* __USB_CORE_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
