#ifndef __USBCORE_H__
#define __USBCORE_H__

#include "stm32f10x.h"

// 标准获取代码(Standard Request Codes)
#define bRequest_GET_STATUS                         0x00
#define bRequest_CLEAR_FEATURE                      0x01
#define bRequest_Reserved                           0x02
#define bRequest_SET_FEATURE                        0x03

#define bRequest_Reserved_Future                    0x04
#define bRequest_SET_ADDRESS                        0x05
#define bRequest_GET_DESCRIPTOR                     0x06
#define bRequest_SET_DESCRIPTOR                     0x07

#define bRequest_GET_CONFIGURATION                  0x08
#define bRequest_SET_CONFIGURATION                  0x09
#define bRequest_SET_IDLE                           0x0A
#define bRequest_GET_INTERFACE                      0x10
#define bRequest_SET_INTERFACE                      0x11
#define bRequest_SYNCH_FRAME                        0x12


#define bRequest_SERIAL_STATE                       0x20
#define bRequest_SET_LINE_CODING                    0x20
#define bRequest_GET_LINE_CODING                    0x21
#define bRequest_SET_CONTROL_LINE_STATE             0x22


// 描述符类型(Descriptor Types)
#define DEVICE_DESCRIPTOR                           1
#define CONFIGURATION_DESCRIPTOR                    2
#define STRING_DESCRIPTOR                           3
#define INTERFACE_DESCRIPTOR                        4
#define ENDPOINT_DESCRIPTOR                         5
#define DEVICE_QUALIFIER_DESCRIPTOR                 6
#define OTHER_SPEED_CONFIGURATION_DESCRIPTOR        7
#define INTERFACE_POWER_DESCRIPTOR                  8
#define REPORT_DESCRIPTOR                           0x22


#pragma pack(1)

typedef struct
{
    uint8_t bmRequestType;
    uint8_t bRequest;
    uint16_t wValue;
    uint16_t wIndex;
    uint16_t wLength;
}Device_Request_t;


typedef struct
{
    /* data */
    uint32_t dwDTERate;                      // baudrate
    uint8_t bCharFormat;                // stop bit(0: 1 stop bit, 1:1.5 stop bit, 2:2 stop bit)
    uint8_t bParityType;                // parity(0:none, 1:Odd, 2:Even, 3:Mark, 4:Space)
    uint8_t bDataBits;                  // data bits(5/6/7/8/16)
}Line_Coding_t;

#pragma pack()

// CDC Line Coding initialization
void Linecoding_Init(void);

// D12 Interrupt Service Routines
void UsbBusSuspend(void);
void UsbBusReset(void);
void UsbEp2In(void);
void UsbEp2Out(void);
void UsbEp1In(void);
void UsbEp1Out(void);
void UsbEp0In(void);
void UsbEp0Out(void);
void UsbEp0SendData(void);


// Read D12 Endpoints buffer data
uint8_t D12ReadEndpointBuffer(uint8_t EpNum, uint8_t Ep_DataLen, uint8_t *Buf);
uint8_t D12WriteEndpointBuffer(uint8_t EpNum, uint8_t Ep_DataLen, uint8_t *Buf);


// Clear Buffer & Validate Buffer
void D12ClearBuffer(void);
void D12ValidateBuffer(void);

// Acknowledge Setup
void D12AckknoledgeSetup(void);

// Endpoint last status
uint8_t D12ReadEndpointLastStatus(uint8_t EpNum);

#endif

