#ifndef __DESCRIPTOR_H__
#define __DESCRIPTOR_H__

#define HID_SUB_DESCRIPTOR_MAX         1

// 变量在内存中连续存放
#pragma pack(1)

typedef struct
{
    uint8_t bLength;                    //描述符数据包字节数
    uint8_t bDescriptorType;            //描述符类型
    uint16_t bcdUSB;                    //BCD码来表示USB的协议版本号
    uint8_t bDeviceClass;               //设备类代码(USB-IF分配)
    uint8_t bDeviceSubClass;            //设备子类代码(USB-IF分配)
    uint8_t bDeviceProtocol;            //设备协议码(由USB-IF分配)
    uint8_t bMaxPacketSize0;            //端点0最大的包长
    uint16_t idVendor;                  //VID(厂商ID由USB-IF分配)
    uint16_t idProduct;                 //产品ID(生产厂商自行分配)
    uint16_t bcdDevice;                 //设备出厂编号
    uint8_t iManufacture;               //设备厂商字符串索引
    uint8_t iProduct;                   //产品描述符字符串索引
    uint8_t iSerialNumber;              //设备序列号字符串索引
    uint8_t bNumConfigurations;         //当前速度下能支持的配置数量
}Device_Descriptor_t;

typedef struct
{
    uint8_t bLength;                    //描述符数据包字节数
    uint8_t bDescriptorType;            //描述符类型
    uint16_t wTotalLength;              //返回描述符集合的数据总长度(配置，接口，端点等)
    uint8_t bNumInterface;              //配置支持的接口数
    uint8_t bConfigurationValue;        //SetConfiguration()获取当前配置的参数值
    uint8_t iConfiguration;             //描述当前配置的字符串描述符的索引值
    uint8_t bmAttributes;               //供电模式选择．Bit4-0保留，D7:总线供电，D6:自供电，D5:远程唤醒
    uint8_t bMaxPower;                  // 设备需要的最大电流量(单位:2mA)
}Configuration_Descriptor_t;

typedef struct
{
    uint8_t bLength;                    //描述符数据包字节数
    uint8_t bDescriptorType;            //描述符类型
    uint8_t bInterfaceNumber;           //当前接口编号
    uint8_t bAlternateSetting;          //前一个确认接口的备用编号
    uint8_t bNumEndpoints;              //该接口所使用的端点数(不包含端点0)
    uint8_t bInterfaceClass;            //该接口使用的类(USB-IF分配)
    uint8_t bInterfaceSubClass;         //该接口使用的子类(USB-IF分配)
    uint8_t bInterfaceProtocol;         //该接口使用的协议(USB-IF分配)
    uint8_t iInterface;                 //描述该接口的字符串索引
}Interface_Descriptor_t;


typedef struct
{
    uint8_t bDescriptorType;            //下级描述符的数量
    uint16_t wDescriptorLength;         //下级描述符的长度
}HID_SubDescriptor_t;


typedef struct
{
    uint8_t bLength;                    //描述符数据包字节数
    uint8_t bDescriptorType;            //描述符类型(HID 0x21)
    uint16_t bcdHID;                     //HID协议版本
    uint8_t bCountryCode;               //国家代码
    uint8_t bNumDescriptors;            //下级描述符的数量
    HID_SubDescriptor_t HID_SubDescriptor[HID_SUB_DESCRIPTOR_MAX];
}HID_Descriptor_t;


typedef struct
{
    uint8_t bLength;                    //描述符数据包字节数
    uint8_t bDescriptorType;            //描述符类型(Endpoint 0x05)
    uint8_t bEndpointAddress;           //该端点地址
    uint8_t bmAttributes;               //该端点属性
    uint16_t wMaxPacketSize;             //该端点支持的最大包长度
    uint8_t bInterval;                  //端点的查询时间
}Endpoint_Descriptor_t;


typedef struct
{
    /* data */
    Configuration_Descriptor_t Configuration_Descriptor;
    Interface_Descriptor_t Interface_Descriptor;
    HID_Descriptor_t HID_Descriptor;
    Endpoint_Descriptor_t Endpoint_Descriptor;
}Descriptor_Set_t;


typedef struct
{
    uint8_t bLength;                    //描述符数据包字节数
    uint8_t bDescriptorType;            //描述符类型(Endpoint 0x05)
    uint16_t wLANGID;   //该端点地址
}LanguageID_Descriptor_t;


#pragma pack()


void Descriptors_Init(void);


#endif


