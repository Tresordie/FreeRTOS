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
    Device_Descriptor.bDeviceClass = 0x02;                                  // CDC Device Class
    Device_Descriptor.bDeviceSubClass = 0;
    Device_Descriptor.bDeviceProtocol = 0;
    Device_Descriptor.bMaxPacketSize0 = 0x10;
    Device_Descriptor.idVendor = 0x8888;
    Device_Descriptor.idProduct = 0x0006;
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
    Descriptor_Set.Configuration_Descriptor.wTotalLength = (uint16_t)(sizeof(Descriptor_Set));
    Descriptor_Set.Configuration_Descriptor.bNumInterface = 0x02;                            //此配置描述符配置两个单独的接口(一个做鼠标，一个做键盘)
    Descriptor_Set.Configuration_Descriptor.bConfigurationValue = 0x01;
    Descriptor_Set.Configuration_Descriptor.iConfiguration = 0x00;
    Descriptor_Set.Configuration_Descriptor.bmAttributes = 0x80;                             //USB总线供电
    Descriptor_Set.Configuration_Descriptor.bMaxPower = 200;                                 //电流200mA

    Descriptor_Set.CDC_Interface_Descriptor.bLength = sizeof(CDC_Interface_Descriptor_t);       //接口描述符数据长度
    Descriptor_Set.CDC_Interface_Descriptor.bDescriptorType = 0x04;                             //接口描述符的编号
    Descriptor_Set.CDC_Interface_Descriptor.bInterfaceNumber = 0x00;                            //该接口的编号 Master Interface
    Descriptor_Set.CDC_Interface_Descriptor.bAlternateSetting = 0x00;                           //该接口的备用编号
    Descriptor_Set.CDC_Interface_Descriptor.bNumEndpoints = 0x01;                               //非0端点的数目(CDC只使用一个非0输入端点作为bulk传输)
    Descriptor_Set.CDC_Interface_Descriptor.bInterfaceClass = 0x02;                             //该接口使用的类编码，CDC类的编码是0x02
    Descriptor_Set.CDC_Interface_Descriptor.bInterfaceSubClass = 0x02;                          //Abstract Control Model子类，编号为0x02
    Descriptor_Set.CDC_Interface_Descriptor.bInterfaceProtocol = 0x01;                          //使用Common AT Commands(通用AT命令)协议，协议编号为0x01
    Descriptor_Set.CDC_Interface_Descriptor.iInterface = 0x00;                                  //该接口的字符串索引值(这里没有，设置为0)

    Descriptor_Set.Header_Function_Descriptor.bFunctionLength = sizeof(Header_Function_Descriptor_t);           //接口描述符数据长度
    Descriptor_Set.Header_Function_Descriptor.bDescriptorType = 0x24;                             //特殊接口类型CS_INTERFACE, 编号为0x24
    Descriptor_Set.Header_Function_Descriptor.bDescriptorSubType = 0x00;                          //该接口的编号
    Descriptor_Set.Header_Function_Descriptor.bcdCDC = 0x1001;                           		  //USB协议版本号

    Descriptor_Set.Call_Management_Function_Descriptor.bFunctionLength = sizeof(Call_Management_Function_Descriptor_t);
    Descriptor_Set.Call_Management_Function_Descriptor.bDescriptorType = 0x24;                 	  //特殊接口类型CS_INTERFACE, 编号为0x24
    Descriptor_Set.Call_Management_Function_Descriptor.bDescriptorSubType = 0x01;                 //子类为Call Management                          
    Descriptor_Set.Call_Management_Function_Descriptor.bmCapabilities = 0x00;       		      //设备自己不管理call management                               
    Descriptor_Set.Call_Management_Function_Descriptor.bDataInterface = 0x00;                     //没有数据类接口作为call management             

    Descriptor_Set.ACM_Functional_Descriptor.bFunctionLength = sizeof(ACM_Functional_Descriptor_t);
    Descriptor_Set.ACM_Functional_Descriptor.bDescriptorType = 0x24;                 	  		  //特殊接口类型CS_INTERFACE, 编号为0x24
    Descriptor_Set.ACM_Functional_Descriptor.bDescriptorSubType = 0x02;                 		  //子类为ACM                         
    Descriptor_Set.ACM_Functional_Descriptor.bmCapabilities = 0x02;       		      			  //D1 - 1(支持Set_Line_Coding, Set_Control_Line_Status, Get_Line_Coding, Serial_State)                              

    Descriptor_Set.Union_Functional_Descriptor.bFunctionLength = sizeof(Union_Functional_Descriptor_t);
    Descriptor_Set.Union_Functional_Descriptor.bDescriptorType = 0x24;                 	  		  //特殊接口类型CS_INTERFACE, 编号为0x24
    Descriptor_Set.Union_Functional_Descriptor.bDescriptorSubType = 0x06;                 		  //子类为Union                          
    Descriptor_Set.Union_Functional_Descriptor.MasterInterface = 0x00;       		      		  //设备自己不管理call management                               
    Descriptor_Set.Union_Functional_Descriptor.SlaveInterface = 0x01;                     		  //接下来编号为1的数据类接口  

	// 接口0使用输入端点1
    Descriptor_Set.Endpoint0_Descriptor.bLength = sizeof(Endpoint_Descriptor_t);
    Descriptor_Set.Endpoint0_Descriptor.bDescriptorType = 0x05;
    Descriptor_Set.Endpoint0_Descriptor.bEndpointAddress = 0x81;                             	  //使用输入端点1
    Descriptor_Set.Endpoint0_Descriptor.bmAttributes = 0x03;                                 	  //D1~D0端点传输类型选择(该端点为中断端点)
    Descriptor_Set.Endpoint0_Descriptor.wMaxPacketSize = 0x0010;                             	  //端点的最大包长(这里设置为16个字节)
    Descriptor_Set.Endpoint0_Descriptor.bInterval = 0x0A;     


    Descriptor_Set.CDC_DataInterface_Descriptor.bLength = sizeof(CDC_Interface_Descriptor_t);       //接口描述符数据长度
    Descriptor_Set.CDC_DataInterface_Descriptor.bDescriptorType = 0x04;                             //接口描述符的编号
    Descriptor_Set.CDC_DataInterface_Descriptor.bInterfaceNumber = 0x01;                            //该接口的编号 Master Interface
    Descriptor_Set.CDC_DataInterface_Descriptor.bAlternateSetting = 0x00;                           //该接口的备用编号
    Descriptor_Set.CDC_DataInterface_Descriptor.bNumEndpoints = 0x02;                               //非0端点的数目(CDC只使用一个非0输入端点作为bulk传输)
    Descriptor_Set.CDC_DataInterface_Descriptor.bInterfaceClass = 0x0A;                             //数据类接口
    Descriptor_Set.CDC_DataInterface_Descriptor.bInterfaceSubClass = 0x00;                         
    Descriptor_Set.CDC_DataInterface_Descriptor.bInterfaceProtocol = 0x00;                          
    Descriptor_Set.CDC_DataInterface_Descriptor.iInterface = 0x00;   

    Descriptor_Set.Endpoint2IN_Descriptor.bLength = sizeof(Endpoint_Descriptor_t);
    Descriptor_Set.Endpoint2IN_Descriptor.bDescriptorType = 0x05;
    Descriptor_Set.Endpoint2IN_Descriptor.bEndpointAddress = 0x82;                             		//Ep2 IN
    Descriptor_Set.Endpoint2IN_Descriptor.bmAttributes = 0x02;   									//Bulk transfer                              
    Descriptor_Set.Endpoint2IN_Descriptor.wMaxPacketSize = 0x0040;     								//Ep2最大包长64bytes                        
    Descriptor_Set.Endpoint2IN_Descriptor.bInterval = 0x00;                							//Bulk transfer无Interval要求                    

    Descriptor_Set.Endpoint2OUT_Descriptor.bLength = sizeof(Endpoint_Descriptor_t);
    Descriptor_Set.Endpoint2OUT_Descriptor.bDescriptorType = 0x05;
    Descriptor_Set.Endpoint2OUT_Descriptor.bEndpointAddress = 0x02;                             	//Ep2 OUT
    Descriptor_Set.Endpoint2OUT_Descriptor.bmAttributes = 0x02;   									//Bulk transfer                              
    Descriptor_Set.Endpoint2OUT_Descriptor.wMaxPacketSize = 0x0040;     							//Ep2最大包长64bytes                        
    Descriptor_Set.Endpoint2OUT_Descriptor.bInterval = 0x00;                						//Bulk transfer无Interval要求                    

    LanguageIDDescriptor_Init();
}
