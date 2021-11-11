#include "usb.h"


//端点1缓冲是否忙的标志。当缓冲区中有数据时，该标志为真。
//当缓冲区中空闲时，该标志为假。
//uint8_t Ep1InIsBusy;
uint8_t Buffer[16] = {0};
uint8_t CDC_Buffer[64] = {0};
uint8_t CDC_DataLenReceived = 0;

//当前的配置值。只有在设置非0配置后才置位
uint8_t ConfigValue = 0;

/** 
 * 是否发送0数据包的标志 
 * 1.在USB控制传输的过程中，当返回的数据包字节数小于最大包长(端点的最大包长)时，就会认为数据过程结束
 * 2.当请求的字节数比实际需要返回的字节数长，而实际返回的字节数又刚好是端点0大小的整数倍时，就需要返回一个长度为0
 *   的数据包来结束数据过程，这里用NeedZeroPacket作为是否需要返回一个0长度数据包的flag
 */
uint8_t NeedZeroPacket;
uint8_t *pSendData;
uint16_t SendLength;
Device_Request_t Device_Request;                            // Usb主机需求数据包

// CDC property setting parameter
Line_Coding_t Line_Coding_Data;

/*
uint8_t LanguageId[4]=
{
 0x04, //本描述符的长度
 0x03, //字符串描述符
 //0x0409为美式英语的ID
 0x09,
 0x04
};
*/

//字符串“USB开发板社区”的Unicode编码
//8位小端格式
uint8_t ManufactureStringDescriptor[18] = {
18,         //该描述符的长度为18字节
0x03,       //字符串描述符的类型编码为0x03
0x55, 0x00, //U
0x53, 0x00, //S
0x42, 0x00, //B
0x00, 0x5f, //开
0xd1, 0x53, //发
0x7f, 0x67, //板
0x3e, 0x79, //社
0x3a, 0x53  //区
};

//字符串“simon.yuan's USB-CDC device”的Unicode编码
//8位小端格式
uint8_t ProductStringDescriptor[56]={
56,         //该描述符的长度为56字节
0x03,       //字符串描述符的类型编码为0x03
0x73, 0x00, //s
0x69, 0x00, //i
0x6d, 0x00, //m
0x6f, 0x00, //o
0x6e, 0x00, //n
0x2e, 0x00, //.
0x79, 0x00, //y
0x75, 0x00, //u
0x61, 0x00, //a
0x6e, 0x00, //n
0x27, 0x00, //'
0x73, 0x00, //s
0x20, 0x00, // 
0x55, 0x00, //U
0x53, 0x00, //S
0x42, 0x00, //B
0x2d, 0x00, //-
0x43, 0x00, //C
0x44, 0x00, //D
0x43, 0x00, //C
0x20, 0x00, // 
0x64, 0x00, //d
0x65, 0x00, //e
0x76, 0x00, //v
0x69, 0x00, //i
0x63, 0x00, //c
0x65, 0x00  //e
};


//字符串“2019-09-05”的Unicode编码
//8位小端格式
uint8_t SerialNumberStringDescriptor[22]={
22,         //该描述符的长度为22字节
0x03,       //字符串描述符的类型编码为0x03
0x32, 0x00, //2
0x30, 0x00, //0
0x31, 0x00, //1
0x39, 0x00, //9
0x2d, 0x00, //-
0x30, 0x00, //0
0x39, 0x00, //9
0x2d, 0x00, //-
0x30, 0x00, //0
0x35, 0x00  //5
};


extern Device_Descriptor_t Device_Descriptor;
extern Descriptor_Set_t Descriptor_Set;
extern LanguageID_Descriptor_t LanguageID_Descriptor;

//static void bmRequestTypeCheck(uint8_t val);
//static void bRequestCheck(uint8_t val);
//static void DescriptorTypesCheck(uint8_t val);
static void USBStandardDeviceRequestCopy(void);
static void D12SetAddress(uint8_t addr);
static void D12SetEndpointEnable(uint8_t Enable);


// line coding struct init
void Linecoding_Init(void)
{
    Line_Coding_Data.dwDTERate = 115200;
    Line_Coding_Data.bCharFormat = 0;
    Line_Coding_Data.bParityType = 0;
    Line_Coding_Data.bDataBits = 8;
}


void UsbBusSuspend(void)
{
    printf("<-----USB挂起中断.----->\r\n");
}


void UsbBusReset(void)
{
    printf("\r\n<=====USB复位中断=====>\r\n");
}


void UsbEp2In(void)
{
    printf("<-----Ep2输入完成中断.----->\r\n");
    D12ReadEndpointLastStatus(5);
    D12ClearBuffer();
}


// Endpoint2 received data from USB Host
void UsbEp2Out(void)
{
    uint8_t i = 0;

    printf("<-----Ep2输出中断.----->\r\n");
    D12ReadEndpointLastStatus(4);                   // clear interrupt flag
    CDC_DataLenReceived = D12ReadEndpointBuffer(4, 64, CDC_Buffer);
     printf("UsbEp2Out received data:\r\n");
    for(i = 0; i < CDC_DataLenReceived; i++)
    {
        if(0 == i % 16){
            printf("\r\n");
        }
        printf("0x%x ", CDC_Buffer[i]);
    }
    D12ClearBuffer();
    D12WriteEndpointBuffer(5, CDC_DataLenReceived, CDC_Buffer);
}


void UsbEp1In(void)
{
    printf("<-----Ep1输入完成中断.----->\r\n");
    D12ReadEndpointLastStatus(3);
}


void UsbEp1Out(void)
{
    uint8_t Buf = 0;                                // LED status control
    printf("<-----Ep1输出中断.----->\r\n");
    D12ReadEndpointLastStatus(2);                   // Clear Interrupt flag
    D12ReadEndpointBuffer(2, 1, &Buf);              // Read Ep1Out buffer
    D12ClearBuffer();                               // Clear buffer

    if(Buf & 0x01){
        LEDsControl(LED1, LED_ON);
    }else{
        LEDsControl(LED1, LED_OFF);
    }

    if(Buf & 0x02){
        LEDsControl(LED2, LED_ON);
    }else{
        LEDsControl(LED2, LED_OFF);
    }

    if(Buf & 0x04){
        LEDsControl(LED3, LED_ON);
    }else{
        LEDsControl(LED3, LED_OFF);
    }

    if(Buf & 0x08){
        LEDsControl(LED4, LED_ON);
    }else{
        LEDsControl(LED4, LED_OFF);
    }
}


// USB Device send data to USB Host and finished
void UsbEp0In(void)
{
    printf("\r\n<-----Ep0输入完成中断.----->\r\n");
    
    // USB设备会在UsbEp0Out中发送一个最大包, 发送完成后进入UsbEp0In中断发送剩余数据
    // 1.清除中断标志位
    // 2.发送剩余数据
    D12ReadEndpointLastStatus(1);
    UsbEp0SendData();
}


// USB host output to USB device
void UsbEp0Out(void)
{
    uint8_t Ep0OutLastStatus;

    printf("<-----Ep0输出中断.----->\r\n");

    // if Ep0 out data is setup packet, need to send ACK to make sure
    // USB host know setup packet had been received successfully
    Ep0OutLastStatus = D12ReadEndpointLastStatus(Ep0_OUT);

    // Setup packet
    if(Ep0OutLastStatus & 0x20)
    {
        printf("通信建立包\r\n");
        D12ReadEndpointBuffer(Ep0_OUT, 16, Buffer);
        D12AckknoledgeSetup();
        D12ClearBuffer();
        USBStandardDeviceRequestCopy();

        //根据bmRequestType的D6~5位散转，D6~5位表示请求的类型
        //0为标准请求，1为类请求，2为厂商请求。
        if((Device_Request.bmRequestType & 0x80) == 0x80)
        {
            switch((Device_Request.bmRequestType >> 5) & 0x03)
            {
                case 0:
                    printf("\r\nUSB标准输入请求====>");
                    switch(Device_Request.bRequest)
                    {
                        case bRequest_GET_CONFIGURATION:
                            printf("获取配置请求：");
                            break;

                        case bRequest_GET_DESCRIPTOR:
                            printf("获取描述符：");
                            switch((Device_Request.wValue >> 8) & 0xFF)
                            {
                                case DEVICE_DESCRIPTOR:
                                    printf("设备描述符.\r\n");
                                    pSendData = (uint8_t *)&Device_Descriptor;
                                    if(Device_Request.wLength > Device_Descriptor.bLength)
                                    {
                                        SendLength = Device_Descriptor.bLength;
                                        if(0 == (SendLength % (Device_Descriptor.bMaxPacketSize0)))
                                        {
                                            NeedZeroPacket = 1;
                                        }
                                    }
                                    else
                                    {
                                        SendLength = Device_Request.wLength;
                                    }
                                    UsbEp0SendData();
                                    break;

                                case CONFIGURATION_DESCRIPTOR:
                                    printf("配置描述符.\r\n");
                                    pSendData = (uint8_t *)&Descriptor_Set;
                                    SendLength = Descriptor_Set.Configuration_Descriptor.wTotalLength;

                                    // 如果请求数据包长度大于实际发送数据包长度，以实际数据包长度为准
                                    if(Device_Request.wLength > SendLength)
                                    {
                                        if(0 == SendLength % Device_Descriptor.bMaxPacketSize0){
                                            NeedZeroPacket = 1;
                                        }
                                    }
                                    else
                                    {
                                        SendLength = Device_Request.wLength;
                                    }
                                    UsbEp0SendData();
                                    break;
                                    
                                case STRING_DESCRIPTOR:
                                    printf("字符串描述符.\r\n");
                                    switch(Device_Request.wValue & 0xFF)
                                    {
                                        case 0:                                     //获取语言ID
                                            printf("Index = 0, 语言ID.");
                                            pSendData = (uint8_t *)&LanguageID_Descriptor;
                                            SendLength = LanguageID_Descriptor.bLength;
                                            break;
                                         
                                        case 1:                                     //厂商字符串的索引值为1，所以这里为厂商字符串
                                            printf("Index = 1, 厂商字符串.");
                                            pSendData = ManufactureStringDescriptor;
                                            SendLength = ManufactureStringDescriptor[0];
                                            break;
                                        
                                        case 2:                                     //产品字符串的索引值为2，所以这里为产品字符串
                                            printf("Index = 2, 产品字符串.");
                                            pSendData = ProductStringDescriptor;
                                            SendLength = ProductStringDescriptor[0];
                                            break;

                                        case 3:                                     //产品字符串的索引值为2，所以这里为产品字符串
                                            printf("Index = 3, 序列号字符串.");
                                            pSendData = SerialNumberStringDescriptor;
                                            SendLength = SerialNumberStringDescriptor[0];
                                            break;

                                        default:
                                            printf("未定义的字符索引.\r\n");
                                            //SendLength = 0;
                                            //NeedZeroPacket = 1;
                                            break;
                                    }

                                    if(Device_Request.wLength > SendLength)
                                    {
                                        if(0 == SendLength % Device_Descriptor.bMaxPacketSize0)
                                        {
                                            NeedZeroPacket = 1;
                                        }
                                    }
                                    else
                                    {
                                        SendLength = Device_Request.wLength;
                                    }
                                    UsbEp0SendData();
                                    break;

                                case REPORT_DESCRIPTOR:                             //报告描述符
                                    printf("报告描述符\r\n");
                                
                                    // 两个硬件接口接收数据，
                                    switch(Device_Request.wIndex)
                                    {
                                        case 0:                 // data request to interface0

                                            break;
                                       
                                        case 1:                 // data request to interface1
                                        
                                            break;
                                    
                                        default:  
                                            SendLength = 0;
                                            NeedZeroPacket = 1;
                                            break;
                                    }
                                   
                                    if(Device_Request.wLength > SendLength)
                                    {
                                        if(0 == SendLength % Device_Descriptor.bMaxPacketSize0){
                                            NeedZeroPacket = 1;
                                        }
                                    }
                                    else{
                                        SendLength = Device_Request.wLength;
                                    }
                                    UsbEp0SendData();
                                    break;

                                default:
                                    printf("其它描述符, 描述符代码为: 0x%x\r\n", ((Device_Request.wValue >> 8) & 0xFF));
                                    break;
                            }
                            break;

                        case bRequest_GET_INTERFACE:
                            printf("获取接口.\r\n");
                            break;

                        case bRequest_GET_STATUS:
                            printf("获取状态.\r\n");
                            break;

                        case bRequest_SYNCH_FRAME:
                            printf("同步帧.\r\n");
                            break;

                        default:
                            printf("错误：未定义的标准输入请求.\r\n");
                            break;    
                    }
                    break;

                case 1:
                    printf("\r\nUSB类输入请求.\r\n");

                    // 类输入请求中的
                    switch(Device_Request.bRequest)
                    {
                        case bRequest_GET_LINE_CODING:
                            printf("\r\nGET_LINE_CODING类输入请求.\r\n");
                            if(Device_Request.wLength > sizeof(Line_Coding_t))
                            {
                                SendLength = sizeof(Line_Coding_t);
                            }
                            else
                            {
                                SendLength = Device_Request.wLength;
                            }
                            printf("SendLength %d bytes.\r\n", SendLength);

                            if(SendLength % Device_Descriptor.bMaxPacketSize0 == 0)
                            {
                                NeedZeroPacket = 1;
                            }                        
                            pSendData = (uint8_t *)&Line_Coding_Data;
                            UsbEp0SendData();
                            break;

                        case bRequest_SERIAL_STATE:
                            printf("\r\bRequest_SERIAL_STATE类输入请求.\r\n");
                            SendLength = 0;
                            NeedZeroPacket = 1;
                            UsbEp0SendData();
                            break;
                    }
                    break;

                 case 2:
                    printf("\r\nUSB厂商输入请求.\r\n");
                    break;

                case 3:
                    printf("\r\n错误：未定义的输入请求.\r\n");
                    break;
            }
        }
        else              //(Device_Request.bmRequestType & 0x80) != 0x80(主机到设备的输出令牌)
        {
            switch((Device_Request.bmRequestType >> 5) & 0x03)
            {
                case 0:
                    printf("\r\nUSB标准输出请求====>");
                    switch(Device_Request.bRequest)
                    {
                        case bRequest_CLEAR_FEATURE:
                            printf("清除特征.\r\n");
                            break;

                        case bRequest_SET_ADDRESS:
                            printf("设置地址：\r\n");
                            printf("0x%x", (Device_Request.wValue & 0xFF));
                            D12SetAddress(Device_Request.wValue & 0xFF);
                        
                            // 设置地址后没有数据返回，直接进入到状态过程，返回一个长度为0的数据包
                            SendLength = 0;
                            NeedZeroPacket = 1;
                            
                            // Ep0发送数据
                            UsbEp0SendData();
                            break;

                        case bRequest_SET_CONFIGURATION:
                            printf("设置配置.\r\n");
                            //使能非0端点。非0端点只有在设置为非0的配置后才能使能。
                            //wValue的低字节为配置的值，如果该值为非0，才能使能非0端点。
                            //保存当前配置值
                            ConfigValue =  Device_Request.wValue & 0xFF;
                            D12SetEndpointEnable(ConfigValue);
                            SendLength = 0;
                            NeedZeroPacket = 1;
                            UsbEp0SendData();
                            break;

                        case bRequest_SET_DESCRIPTOR:
                            printf("设置描述符.\r\n");
                            break;

                        case bRequest_SET_FEATURE:
                            printf("设置特征.\r\n");
                            break;    
                        
                        case bRequest_SET_INTERFACE:
                            printf("设置接口.\r\n");
                            break;

                        default:
                            printf("错误：未定义的标准输出请求.\r\n");
                            break;
                    }
                    break;

                case 1:
                    printf("\r\nUSB类输出请求====>");
                    switch(Device_Request.bRequest)
                    {
                        case bRequest_SET_IDLE:
                            printf("设置空闲.\r\n");
                            // 只需要返回一个0长度的数据包即可
                            SendLength = 0;
                            NeedZeroPacket = 1;
                            UsbEp0SendData();
                            break;

                        //该请求设置串口的属性，但是实际的数据并不在设置过程发出，
                        //而是在之后的数据过程发出。这里不用做任何处理，在数据过程
                        //完成后返回0长度的状态包。
                        case bRequest_SET_LINE_CODING:
                            printf("\r\nSET_LINE_CODING类输出请求.\r\n");
                            break;    

                        //该请求没有数据输出阶段，其中wValue字段的D0位表示DTR，
                        //D1位表示RTS。但是我们的板上的串口并没有这两引脚，因而
                        //对该请求我们仅是简单地返回一个0长度的状态过程数据包即可
                        case bRequest_SET_CONTROL_LINE_STATE:
                            printf("\r\nSET_CONTROL_LINE_STATE类输出请求.\r\n");
                            SendLength = 0;
                            NeedZeroPacket = 1;
                            UsbEp0SendData();
                            break;


                        default:
                            printf("未知请求.\r\n");
                    }
                    break;

                case 2:
                    printf("\r\nUSB厂商输出请求====>：\r\n");
                    break;
                case 3:
                    printf("\r\n错误: 未定义输出请求.\r\n");
                    break;
            }
        }
    }
    else        // Not setup packet
    {
        printf("普通数据包\r\n");
        D12ReadEndpointBuffer(Ep0_OUT, 16, Buffer);
        D12ClearBuffer();
    }
}


/********************************************************************
 *函数功能：根据pData和SendLength将数据发送到端点0的函数。
 *入口参数：无。
 *返     回：无。
 *备     注：无。
 *******************************************************************/
void UsbEp0SendData(void)
{
    // 数据通过Ep0发送出去，最大发送的包长为端点0限定的最大包长
    if(SendLength > Device_Descriptor.bMaxPacketSize0)
    {
        D12WriteEndpointBuffer(1, Device_Descriptor.bMaxPacketSize0, pSendData);
        SendLength -= Device_Descriptor.bMaxPacketSize0;
        pSendData += Device_Descriptor.bMaxPacketSize0;
    }
    else
    {
        if(SendLength != 0)
        {
            D12WriteEndpointBuffer(1, SendLength, pSendData);
            SendLength = 0;
        }
        else
        {
            if(1 == NeedZeroPacket)
            {
                D12WriteEndpointBuffer(1, 0, pSendData);
                NeedZeroPacket = 0;
            }
        }
    }
}


/**
 *
 *  读端点x的数据
 *  (1)首先需要选取待读取数据的端点.
 *  (2)读取指定端点的数据并存放到相应的缓冲区.
 *
 */
static void D12SelectEndpoint(uint8_t EpNum)
{
    D12WriteCommand(0x00 + EpNum);
}


/// Read D12 Endpoints buffer data
uint8_t D12ReadEndpointBuffer(uint8_t EpNum, uint8_t Ep_DataLen, uint8_t *Buf)
{
    uint8_t i, actual_receivedLen, sendout_dataLen;
    D12SelectEndpoint(EpNum);
    D12WriteCommand(D12_REGISTER_ReadBuffer);
    D12ReadData();                                  // Random data
    actual_receivedLen = D12ReadData();                              // Ep buffer data len received

    if(actual_receivedLen > Ep_DataLen)                              // actual received dataLen > request dataLen
    {
        sendout_dataLen = Ep_DataLen;                             // Ep_DataLen MCU buffer, j USB PHY EpNum buffer received data length
    }
    else
    {
        sendout_dataLen = actual_receivedLen;
    }
    
    printf("Read Ep%d buffer %d bytes.\r\n",(EpNum/2), sendout_dataLen);

    D12SetPortIn();
    D12SetDataAddr();
    for(i = 0; i < sendout_dataLen; i++)
    {
        D12ClearRD();
        *(Buf + i) = GPIO_ReadInputData(D12_DATA_PORT);
        D12SetRD();

        printf("0x%02x ", *(Buf + i));
        if(0 == ((i + 1) % 16))
        {
            printf("\r\n");
        }
    }
    return sendout_dataLen;
}


/// Write D12 Endpoints buffer data
uint8_t D12WriteEndpointBuffer(uint8_t EpNum, uint8_t Ep_DataLen, uint8_t *Buf)
{
    uint8_t i;
    D12SelectEndpoint(EpNum);
    D12WriteCommand(D12_REGISTER_WriteBuffer);

    D12WriteData(0);
    D12WriteData(Ep_DataLen);

    printf("Write Ep%d buffer %d bytes.\r\n", (EpNum/2), Ep_DataLen);

    D12SetPortOut();
    D12SetDataAddr();
    for(i = 0; i < Ep_DataLen; i++)
    {
        D12ClearWR();
        D12SetData(*(Buf + i));
        D12SetWR();

        printf("0x%02x ", *(Buf + i));
        if(0 == ((i + 1) % 16))
        {
            printf("\r\n");
        }
    }

    D12SetPortIn();
    D12ValidateBuffer();

    return Ep_DataLen;
}


/// Clear Buffer & Validate Buffer
void D12ClearBuffer(void)
{
    D12WriteCommand(D12_REGISTER_ClearBuffer);
}

void D12ValidateBuffer(void)
{
    D12WriteCommand(D12_REGISTER_ValidateBuffer);
}

/// Acknowledge Setup
void D12AckknoledgeSetup(void)
{
    // 这里必须要先选择端点1(0 -> OUT, 1 -> IN)
    D12SelectEndpoint(1);                                           // USB端点0输入
    D12WriteCommand(D12_REGISTER_AcknowledgeSetup);                 
    D12SelectEndpoint(0);                                           // USB端点0输出，这里的选择的端点与后面的Clear buffer命令对应
    D12WriteCommand(D12_REGISTER_AcknowledgeSetup);
}


/// Read last transaction status register
uint8_t D12ReadEndpointLastStatus(uint8_t EpNum)
{
    uint8_t ret = 0;

    switch(EpNum)
    {
        case Ep0_OUT:
            D12WriteCommand(D12_REGISTER_Ep0_OUT_LastTransactionStatus);
            break;

        case Ep0_IN:
            D12WriteCommand(D12_REGISTER_Ep0_IN_LastTransactionStatus);
            break;

        case Ep1_OUT:
            D12WriteCommand(D12_REGISTER_Ep1_OUT_LastTransactionStatus);
            break;

        case Ep1_IN:
            D12WriteCommand(D12_REGISTER_Ep1_IN_LastTransactionStatus);
            break;

        case Ep2_OUT:
            D12WriteCommand(D12_REGISTER_Ep2_OUT_LastTransactionStatus);
            break;
        
        case Ep2_IN:
            D12WriteCommand(D12_REGISTER_Ep2_IN_LastTransactionStatus);
            break;
    }

    ret = D12ReadData();

    return ret;
}


/// Standard Device Requests category
static void USBStandardDeviceRequestCopy(void)
{
    memset(&Device_Request, 0, sizeof(Device_Request));
    Device_Request.bmRequestType = Buffer[0];
    Device_Request.bRequest = Buffer[1];
    Device_Request.wValue = Buffer[2] + (Buffer[3] << 8);
    Device_Request.wIndex = Buffer[4] + (Buffer[5] << 8);
    Device_Request.wLength = Buffer[6] + (Buffer[7] << 8);
}


static void D12SetAddress(uint8_t addr)
{
    D12WriteCommand(D12_REGISTER_SetAddress);
    D12WriteData(0x80 | addr);
}


static void D12SetEndpointEnable(uint8_t Enable)
{
    D12WriteCommand(D12_REGISTER_SetEndpointEnable);
    if(0 != Enable){
        D12WriteData(0x01);  
    }else{
        D12WriteData(0x00);  
    }
     
}

