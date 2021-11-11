#include "usb.h"

Device_Descriptor_t Device_Descriptor;
Descriptor_Set_t Descriptor_Set;
LanguageID_Descriptor_t LanguageID_Descriptor;

// Initialize device descriptor
static void DeviceDescriptor_Init(void)
{
    Device_Descriptor.bLength = sizeof(Device_Descriptor_t);
    Device_Descriptor.bDescriptorType = 0x01;
    Device_Descriptor.bcdUSB = 0x0110;
    Device_Descriptor.bDeviceClass = 0;
    Device_Descriptor.bDeviceSubClass = 0;
    Device_Descriptor.bDeviceProtocol = 0;
    Device_Descriptor.bMaxPacketSize0 = 0x10;
    Device_Descriptor.idVendor = 0x8888;
    Device_Descriptor.idProduct = 0x0003;
    Device_Descriptor.bcdDevice = 0x0100;
    Device_Descriptor.iManufacture = 0x01;
    Device_Descriptor.iProduct = 0x02;
    Device_Descriptor.iSerialNumber = 0x03;
    Device_Descriptor.bNumConfigurations = 0x01;
}


static void LanguageIDDescriptor_Init(void)
{
    LanguageID_Descriptor.bLength = sizeof(LanguageID_Descriptor_t);
    LanguageID_Descriptor.bDescriptorType = 0x03;               //字符串描述符0x03
    LanguageID_Descriptor.wLANGID = 0x0409;                     //美式英语ID
}


void Descriptors_Init(void)
{
    DeviceDescriptor_Init();

    Descriptor_Set.Configuration_Descriptor.bLength = sizeof(Configuration_Descriptor_t);
    Descriptor_Set.Configuration_Descriptor.bDescriptorType = 0x02;
    Descriptor_Set.Configuration_Descriptor.wTotalLength = (uint16_t)(sizeof(Configuration_Descriptor_t) + sizeof(Interface_Descriptor_t) + sizeof(HID_Descriptor_t) + sizeof(Endpoint_Descriptor_t) + sizeof(Endpoint_Descriptor_t));
    Descriptor_Set.Configuration_Descriptor.bNumInterface = 0x01;
    Descriptor_Set.Configuration_Descriptor.bConfigurationValue = 0x01;
    Descriptor_Set.Configuration_Descriptor.iConfiguration = 0x00;
    Descriptor_Set.Configuration_Descriptor.bmAttributes = 0x80;                            //USB总线供电
    Descriptor_Set.Configuration_Descriptor.bMaxPower = 200;                                //电流200mA

    Descriptor_Set.Interface_Descriptor.bLength = sizeof(Interface_Descriptor_t);           //接口描述符数据长度
    Descriptor_Set.Interface_Descriptor.bDescriptorType = 0x04;                             //接口描述符的编号
    Descriptor_Set.Interface_Descriptor.bInterfaceNumber = 0x00;                            //该接口的编号
    Descriptor_Set.Interface_Descriptor.bAlternateSetting = 0x00;                           //该接口的备用编号
    Descriptor_Set.Interface_Descriptor.bNumEndpoints = 0x02;                               //非0端点的数目(USB鼠标只需要一个中断输入端点, 因此设置为1; USB键盘使用了两个端点)
    Descriptor_Set.Interface_Descriptor.bInterfaceClass = 0x03;                             //该接口使用的类编码，HID类的编码是0x03
    Descriptor_Set.Interface_Descriptor.bInterfaceSubClass = 0x01;                          //引导启动的子类.USB键盘，鼠标属于该子类,子类代码是0x01
    Descriptor_Set.Interface_Descriptor.bInterfaceProtocol = 0x01;                          //如果子类为支持引导启动的子类，则协议可选择鼠标和键盘，键盘代码为0x01, 鼠标代码为0x02
    Descriptor_Set.Interface_Descriptor.iInterface = 0x00;                                  //该接口的字符串索引值(这里没有，设置为0)

    Descriptor_Set.HID_Descriptor.bLength = sizeof(HID_Descriptor_t);
    Descriptor_Set.HID_Descriptor.bDescriptorType = 0x21;                                   //HID描述符的编号为0x21
    Descriptor_Set.HID_Descriptor.bcdHID = 0x0110;                                          //HID1.1协议
    Descriptor_Set.HID_Descriptor.bCountryCode = 0x21;                                      //设备适用的国家代码为美国, 0x21
    Descriptor_Set.HID_Descriptor.bNumDescriptors = 0x01;                                   //下级描述符的数目,只有一个报告描述符
    Descriptor_Set.HID_Descriptor.HID_SubDescriptor[0].bDescriptorType = 0x22;              //下级描述符类型为报告描述符，编号为0x22
    Descriptor_Set.HID_Descriptor.HID_SubDescriptor[0].wDescriptorLength = 105;
    Descriptor_Set.Endpoint_Descriptor1.bLength = sizeof(Endpoint_Descriptor_t);
    Descriptor_Set.Endpoint_Descriptor1.bDescriptorType = 0x05;
    Descriptor_Set.Endpoint_Descriptor1.bEndpointAddress = 0x81;                             //使用输入端点1
    Descriptor_Set.Endpoint_Descriptor1.bmAttributes = 0x03;                                 //D1~D0端点传输类型选择(该端点为中断端点)
    Descriptor_Set.Endpoint_Descriptor1.wMaxPacketSize = 0x0010;                             //端点的最大包长(这里设置为16个字节)
    Descriptor_Set.Endpoint_Descriptor1.bInterval = 0x0A;                                    //端点查询时间为10个帧间隔时间即10ms

    Descriptor_Set.Endpoint_Descriptor2.bLength = sizeof(Endpoint_Descriptor_t);
    Descriptor_Set.Endpoint_Descriptor2.bDescriptorType = 0x05;
    Descriptor_Set.Endpoint_Descriptor2.bEndpointAddress = 0x01;                             //使用输出端点1
    Descriptor_Set.Endpoint_Descriptor2.bmAttributes = 0x03;                                 //D1~D0端点传输类型选择(该端点为中断端点)
    Descriptor_Set.Endpoint_Descriptor2.wMaxPacketSize = 0x0010;                             //端点的最大包长(这里设置为16个字节)
    Descriptor_Set.Endpoint_Descriptor2.bInterval = 0x0A;                                    //端点查询时间为10个帧间隔时间即10ms

    LanguageIDDescriptor_Init();
}


