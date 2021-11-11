#include "usb.h"
#include "bsp.h"


//端点1缓冲是否忙的标志。当缓冲区中有数据时，该标志为真。
//当缓冲区中空闲时，该标志为假。
uint8_t Ep1InIsBusy;
uint8_t Buffer[16] = {0};

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


/*
//字符串“simon.yuan's USB Mouse”的Unicode编码
//8位小端格式
uint8_t ProductStringDescriptor[46] = {
46,         //该描述符的长度为46字节
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
0x20, 0x00, // 
0x4d, 0x00, //M
0x6f, 0x00, //o
0x75, 0x00, //u
0x73, 0x00, //s
0x65, 0x00, //e
//注：请将最后一个多余的逗号删除
};
*/

//字符串“simon.yuan's Keyboard”的Unicode编码
//8位小端格式
uint8_t ProductStringDescriptor[44]={
44,         //该描述符的长度为44字节
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
0x4b, 0x00, //K
0x65, 0x00, //e
0x79, 0x00, //y
0x62, 0x00, //b
0x6f, 0x00, //o
0x61, 0x00, //a
0x72, 0x00, //r
0x64, 0x00  //d
};

/*
//字符串“2019-09-02”的Unicode编码
//8位小端格式
uint8_t SerialNumberStringDescriptor[22] = {
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
0x32, 0x00  //2
};
*/

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


/*
// Keyboard support 6 keys(A ~ F)
uint8_t ReportDescriptor[51] = {
    0x05, 0x01,                    // USAGE_PAGE (Generic Desktop)
    0x09, 0x06,                    // USAGE (Keyboard)
    0xa1, 0x01,                    // COLLECTION (Application)
    0x05, 0x07,                    //   USAGE_PAGE (Keyboard)
    0x19, 0x04,                    //   USAGE_MINIMUM (Keyboard a and A)
    0x29, 0x09,                    //   USAGE_MAXIMUM (Keyboard f and F)
    0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
    0x25, 0x01,                    //   LOGICAL_MAXIMUM (1)
    0x75, 0x01,                    //   REPORT_SIZE (1)
    0x95, 0x06,                    //   REPORT_COUNT (6)
    0x81, 0x00,                    //   INPUT (Data,Ary,Abs)
    0x75, 0x02,                    //   REPORT_SIZE (2)
    0x95, 0x01,                    //   REPORT_COUNT (1)
    0x81, 0x01,                    //   INPUT (Cnst,Ary,Abs)
    0x05, 0x08,                    //   USAGE_PAGE (LEDs)
    0x19, 0x01,                    //   USAGE_MINIMUM (Num Lock)
    0x29, 0x03,                    //   USAGE_MAXIMUM (Scroll Lock)
    0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
    0x25, 0x01,                    //   LOGICAL_MAXIMUM (1)
    0x75, 0x01,                    //   REPORT_SIZE (1)
    0x95, 0x03,                    //   REPORT_COUNT (3)
    0x91, 0x00,                    //   OUTPUT (Data,Ary,Abs)
    0x75, 0x05,                    //   REPORT_SIZE (5)
    0x95, 0x01,                    //   REPORT_COUNT (1)
    0x91, 0x01,                    //   OUTPUT (Cnst,Ary,Abs)
    0xc0                           // END_COLLECTION
};
*/

uint8_t ReportDescriptor[45] = {
    0x05, 0x01,                    // USAGE_PAGE (Generic Desktop)
    0x09, 0x06,                    // USAGE (Keyboard)
    0xa1, 0x01,                    // COLLECTION (Application)
    0x05, 0x07,                    //   USAGE_PAGE (Keyboard)
    0x19, 0x04,                    //   USAGE_MINIMUM (Keyboard a and A)
    0x29, 0x0b,                    //   USAGE_MAXIMUM (Keyboard h and H)
    0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
    0x25, 0x01,                    //   LOGICAL_MAXIMUM (1)
    0x75, 0x01,                    //   REPORT_SIZE (1)
    0x95, 0x08,                    //   REPORT_COUNT (8)
    0x81, 0x00,                    //   INPUT (Data,Ary,Abs)
    0x05, 0x08,                    //   USAGE_PAGE (LEDs)
    0x19, 0x01,                    //   USAGE_MINIMUM (Num Lock)
    0x29, 0x03,                    //   USAGE_MAXIMUM (Scroll Lock)
    0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
    0x25, 0x01,                    //   LOGICAL_MAXIMUM (1)
    0x75, 0x01,                    //   REPORT_SIZE (1)
    0x95, 0x03,                    //   REPORT_COUNT (3)
    0x91, 0x00,                    //   OUTPUT (Data,Ary,Abs)
    0x75, 0x05,                    //   REPORT_SIZE (5)
    0x95, 0x01,                    //   REPORT_COUNT (1)
    0x91, 0x01,                    //   OUTPUT (Cnst,Ary,Abs)
    0xc0                           // END_COLLECTION
};


/*
//USB报告描述符的定义
uint8_t ReportDescriptor[65] =
{
 //每行开始的第一字节为该条目的前缀，前缀的格式为：
 //D7~D4：bTag。D3~D2：bType；D1~D0：bSize。以下分别对每个条目注释。
 
 //这是一个全局（bType为1）条目，将用途页选择为普通桌面Generic Desktop Page(0x01)
 //后面跟一字节数据（bSize为1），后面的字节数就不注释了，
 //自己根据bSize来判断。
 0x05, 0x01, // USAGE_PAGE (Generic Desktop)
 
 //这是一个局部（bType为2）条目，说明接下来的集合用途用于键盘
 0x09, 0x06, // USAGE (Keyboard)
 
 //这是一个主条目（bType为0）条目，开集合，后面跟的数据0x01表示
 //该集合是一个应用集合。它的性质在前面由用途页和用途定义为
 //普通桌面用的键盘。
 0xa1, 0x01, // COLLECTION (Application)
 
 //这是一个全局条目，选择用途页为键盘（Keyboard/Keypad(0x07)）
 0x05, 0x07, //     USAGE_PAGE (Keyboard/Keypad)

 //这是一个局部条目，说明用途的最小值为0xe0。实际上是键盘左Ctrl键。
 //具体的用途值可在HID用途表中查看。
 0x19, 0xe0, //     USAGE_MINIMUM (Keyboard LeftControl)
 
 //这是一个局部条目，说明用途的最大值为0xe7。实际上是键盘右GUI键。
 0x29, 0xe7, //     USAGE_MAXIMUM (Keyboard Right GUI)
 
 //这是一个全局条目，说明返回的数据的逻辑值（就是我们返回的数据域的值）
 //最小为0。因为我们这里用Bit来表示一个数据域，因此最小为0，最大为1。
 0x15, 0x00, //     LOGICAL_MINIMUM (0)
 
 //这是一个全局条目，说明逻辑值最大为1。
 0x25, 0x01, //     LOGICAL_MAXIMUM (1)
 
 //这是一个全局条目，说明数据域的数量为八个。
 0x95, 0x08, //     REPORT_COUNT (8)
 
 //这是一个全局条目，说明每个数据域的长度为1个bit。
 0x75, 0x01, //     REPORT_SIZE (1)
 
 //这是一个主条目，说明有8个长度为1bit的数据域（数量和长度
 //由前面的两个全局条目所定义）用来做为输入，
 //属性为：Data,Var,Abs。Data表示这些数据可以变动，Var表示
 //这些数据域是独立的，每个域表示一个意思。Abs表示绝对值。
 //这样定义的结果就是，当某个域的值为1时，就表示对应的键按下。
 //bit0就对应着用途最小值0xe0，bit7对应着用途最大值0xe7。
 0x81, 0x02, //     INPUT (Data,Var,Abs)
 
 //这是一个全局条目，说明数据域数量为1个
 0x95, 0x01, //     REPORT_COUNT (1)
 
 //这是一个全局条目，说明每个数据域的长度为8bit。
 0x75, 0x08, //     REPORT_SIZE (8)
 
 //这是一个主条目，输入用，由前面两个全局条目可知，长度为8bit，
 //数量为1个。它的属性为常量（即返回的数据一直是0）。
 //该字节是保留字节（保留给OEM使用）。
 0x81, 0x03, //     INPUT (Cnst,Var,Abs)
 
 //这是一个全局条目。定义位域数量为6个。
 0x95, 0x06, //   REPORT_COUNT (6)
 
 //这是一个全局条目。定义每个位域长度为8bit。
 //其实这里这个条目不要也是可以的，因为在前面已经有一个定义
 //长度为8bit的全局条目了。
 0x75, 0x08, //   REPORT_SIZE (8)
 
 //这是一个全局条目，定义逻辑最小值为0。
 //同上，这里这个全局条目也是可以不要的，因为前面已经有一个
 //定义逻辑最小值为0的全局条目了。
 0x15, 0x00, //   LOGICAL_MINIMUM (0)
 
 //这是一个全局条目，定义逻辑最大值为255。
 0x25, 0xFF, //   LOGICAL_MAXIMUM (255)
 
 //这是一个全局条目，选择用途页为键盘。
 //前面已经选择过用途页为键盘了，所以该条目不要也可以。
 0x05, 0x07, //   USAGE_PAGE (Keyboard/Keypad)
 
 //这是一个局部条目，定义用途最小值为0（0表示没有键按下）
 0x19, 0x00, //   USAGE_MINIMUM (Reserved (no event indicated))
 
 //这是一个局部条目，定义用途最大值为0x65
 0x29, 0x65, //   USAGE_MAXIMUM (Keyboard Application)
 
 //这是一个主条目。它说明这六个8bit的数据域是输入用的，
 //属性为：Data,Ary,Abs。Data说明数据是可以变的，Ary说明
 //这些数据域是一个数组，即每个8bit都可以表示某个键值，
 //如果按下的键太多（例如超过这里定义的长度或者键盘本身无法
 //扫描出按键情况时），则这些数据返回全1（二进制），表示按键无效。
 //Abs表示这些值是绝对值。
 0x81, 0x00, //     INPUT (Data,Ary,Abs)

 //以下为输出报告的描述
 //逻辑最小值前面已经有定义为0了，这里可以省略。 
 //这是一个全局条目，说明逻辑值最大为1。
 0x25, 0x01, //     LOGICAL_MAXIMUM (1)
 
 //这是一个全局条目，说明数据域数量为5个。 
 0x95, 0x05, //   REPORT_COUNT (5)
 
 //这是一个全局条目，说明数据域的长度为1bit。
 0x75, 0x01, //   REPORT_SIZE (1)
 
 //这是一个全局条目，说明使用的用途页为指示灯（LED）
 0x05, 0x08, //   USAGE_PAGE (LEDs)
 
 //这是一个局部条目，说明用途最小值为数字键盘灯。
 0x19, 0x01, //   USAGE_MINIMUM (Num Lock)
 
 //这是一个局部条目，说明用途最大值为Kana灯。
 0x29, 0x05, //   USAGE_MAXIMUM (Kana)
 
 //这是一个主条目。定义输出数据，即前面定义的5个LED。
 0x91, 0x02, //   OUTPUT (Data,Var,Abs)
 
 //这是一个全局条目。定义位域数量为1个。
 0x95, 0x01, //   REPORT_COUNT (1)
 
 //这是一个全局条目。定义位域长度为3bit。
 0x75, 0x03, //   REPORT_SIZE (3)
 
 //这是一个主条目，定义输出常量，前面用了5bit，所以这里需要
 //3个bit来凑成一字节。
 0x91, 0x03, //   OUTPUT (Cnst,Var,Abs)
 
 //下面这个主条目用来关闭前面的集合。bSize为0，所以后面没数据。
 0xc0        // END_COLLECTION
};
//通过上面的报告描述符的定义，我们知道返回的输入报告具有8字节。
//第一字节的8个bit用来表示特殊键是否按下（例如Shift、Alt等键）。
//第二字节为保留值，值为常量0。第三到第八字节是一个普通键键值的
//数组，当没有键按下时，全部6个字节值都为0。当只有一个普通键按下时，
//这六个字节中的第一字节值即为该按键的键值（具体的键值请看HID的
//用途表文档），当有多个普通键同时按下时，则同时返回这些键的键值。
//如果按下的键太多，则这六个字节都为0xFF（不能返回0x00，这样会让
//操作系统认为所有键都已经释放）。至于键值在数组中的先后顺序是
//无所谓的，操作系统会负责检查是否有新键按下。我们应该在中断端点1
//中按照上面的格式返回实际的键盘数据。另外，报告中还定义了一个字节
//的输出报告，是用来控制LED情况的。只使用了低7位，高1位是保留值0。
//当某位的值为1时，则表示对应的LED要点亮。操作系统会负责同步各个
//键盘之间的LED，例如你有两块键盘，一块的数字键盘灯亮时，另一块
//也会跟着亮。键盘本身不需要判断各种LED应该何时亮，它只是等待主机
//发送报告给它，然后根据报告值来点亮相应的LED。我们在端点1输出中断
//中读出这1字节的输出报告，然后对它取反（因为学习板上的LED是低电平时
//亮），直接发送到LED上。这样main函数中按键点亮LED的代码就不需要了。
///////////////////////////报告描述符完毕////////////////////////////
*/

extern Device_Descriptor_t Device_Descriptor;
extern Descriptor_Set_t Descriptor_Set;
extern LanguageID_Descriptor_t LanguageID_Descriptor;

static void USBStandardDeviceRequestCopy(void);
static void D12SetAddress(uint8_t addr);
static void D12SetEndpointEnable(uint8_t Enable);

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
}


void UsbEp2Out(void)
{
    printf("<-----Ep2输出中断.----->\r\n");
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
        LED1_ON;
    }else{
        LED1_OFF;
    }

    if(Buf & 0x02){
        LED2_ON;
    }else{
        LED2_OFF;
    }

    if(Buf & 0x04){
        LED3_ON;
    }else{
        LED3_OFF;
    }

    if(Buf & 0x08){
        LED4_ON;
    }else{
        LED4_OFF;
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

    printf("\r\n<-----Ep0输出中断.----->\r\n");

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
                    printf("\r\nUSB标准输入请求====>\r\n");
                    switch(Device_Request.bRequest)
                    {
                        case bRequest_GET_CONFIGURATION:
                            printf("获取配置请求：\r\n");
                            break;

                        case bRequest_GET_DESCRIPTOR:
                            printf("获取描述符：\r\n");
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
                                            printf("Index = 0, 语言ID.\r\n");
                                            //pSendData = LanguageId;
                                            //SendLength = LanguageId[0];
                                            pSendData = (uint8_t *)&LanguageID_Descriptor;
                                            SendLength = LanguageID_Descriptor.bLength;
                                            break;
                                         
                                        case 1:                                     //厂商字符串的索引值为1，所以这里为厂商字符串
                                            printf("Index = 1, 厂商字符串.\r\n");
                                            pSendData = ManufactureStringDescriptor;
                                            SendLength = ManufactureStringDescriptor[0];
                                            break;
                                        
                                        case 2:                                     //产品字符串的索引值为2，所以这里为产品字符串
                                            printf("Index = 2, 产品字符串.\r\n");
                                            pSendData = ProductStringDescriptor;
                                            SendLength = ProductStringDescriptor[0];
                                            break;

                                        case 3:                                     //产品字符串的索引值为2，所以这里为产品字符串
                                            printf("Index = 3, 序列号字符串.\r\n");
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
                                    pSendData = ReportDescriptor;
                                    SendLength = sizeof(ReportDescriptor);
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
                    printf("\r\nUSB标准输出请求====>\r\n");
                    switch(Device_Request.bRequest)
                    {
                        case bRequest_CLEAR_FEATURE:
                            printf("清除特征.\r\n");
                            break;

                        case bRequest_SET_ADDRESS:
                            printf("设置地址：\r\n");
                            printf("0x%x\r\n", (Device_Request.wValue & 0xFF));
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
                    printf("\r\nUSB类输出请求====>\r\n");
                    switch(Device_Request.bRequest)
                    {
                        case bRequest_SET_IDLE:
                            printf("设置空闲.\r\n");
                            // 只需要返回一个0长度的数据包即可
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
        printf("普通数据包");
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
    uint8_t i, j;
    D12SelectEndpoint(EpNum);
    D12WriteCommand(D12_REGISTER_ReadBuffer);
    D12ReadData();                                  // Random data
    j = D12ReadData();                              // Ep buffer data len received

    if(j >= Ep_DataLen)
    {
        j = Ep_DataLen;                             // Ep_DataLen MCU buffer, j USB PHY EpNum buffer received data length
    }
    printf("Read Ep%d buffer %d bytes\r\n.",(EpNum/2), j);

    D12SetPortIn();
    D12SetDataAddr();
    for(i = 0; i < j; i++)
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
    return j;
}


/// Write D12 Endpoints buffer data
uint8_t D12WriteEndpointBuffer(uint8_t EpNum, uint8_t Ep_DataLen, uint8_t *Buf)
{
    uint8_t i;
    D12SelectEndpoint(EpNum);
    D12WriteCommand(D12_REGISTER_WriteBuffer);

    D12WriteData(0);
    D12WriteData(Ep_DataLen);

    printf("Write Ep%d buffer %d bytes\r\n", (EpNum/2), Ep_DataLen);

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

