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
    LanguageID_Descriptor.bDescriptorType = 0x03;               //�ַ���������0x03
    LanguageID_Descriptor.wLANGID = 0x0409;                     //��ʽӢ��ID
}


void Descriptors_Init(void)
{
    DeviceDescriptor_Init();

    Descriptor_Set.Configuration_Descriptor.bLength = sizeof(Configuration_Descriptor_t);
    Descriptor_Set.Configuration_Descriptor.bDescriptorType = 0x02;
    Descriptor_Set.Configuration_Descriptor.wTotalLength = (uint16_t)(sizeof(Descriptor_Set));
    Descriptor_Set.Configuration_Descriptor.bNumInterface = 0x02;                            //�������������������������Ľӿ�(һ������꣬һ��������)
    Descriptor_Set.Configuration_Descriptor.bConfigurationValue = 0x01;
    Descriptor_Set.Configuration_Descriptor.iConfiguration = 0x00;
    Descriptor_Set.Configuration_Descriptor.bmAttributes = 0x80;                             //USB���߹���
    Descriptor_Set.Configuration_Descriptor.bMaxPower = 200;                                 //����200mA

    Descriptor_Set.CDC_Interface_Descriptor.bLength = sizeof(CDC_Interface_Descriptor_t);       //�ӿ����������ݳ���
    Descriptor_Set.CDC_Interface_Descriptor.bDescriptorType = 0x04;                             //�ӿ��������ı��
    Descriptor_Set.CDC_Interface_Descriptor.bInterfaceNumber = 0x00;                            //�ýӿڵı�� Master Interface
    Descriptor_Set.CDC_Interface_Descriptor.bAlternateSetting = 0x00;                           //�ýӿڵı��ñ��
    Descriptor_Set.CDC_Interface_Descriptor.bNumEndpoints = 0x01;                               //��0�˵����Ŀ(CDCֻʹ��һ����0����˵���Ϊbulk����)
    Descriptor_Set.CDC_Interface_Descriptor.bInterfaceClass = 0x02;                             //�ýӿ�ʹ�õ�����룬CDC��ı�����0x02
    Descriptor_Set.CDC_Interface_Descriptor.bInterfaceSubClass = 0x02;                          //Abstract Control Model���࣬���Ϊ0x02
    Descriptor_Set.CDC_Interface_Descriptor.bInterfaceProtocol = 0x01;                          //ʹ��Common AT Commands(ͨ��AT����)Э�飬Э����Ϊ0x01
    Descriptor_Set.CDC_Interface_Descriptor.iInterface = 0x00;                                  //�ýӿڵ��ַ�������ֵ(����û�У�����Ϊ0)

    Descriptor_Set.Header_Function_Descriptor.bFunctionLength = sizeof(Header_Function_Descriptor_t);           //�ӿ����������ݳ���
    Descriptor_Set.Header_Function_Descriptor.bDescriptorType = 0x24;                             //����ӿ�����CS_INTERFACE, ���Ϊ0x24
    Descriptor_Set.Header_Function_Descriptor.bDescriptorSubType = 0x00;                          //�ýӿڵı��
    Descriptor_Set.Header_Function_Descriptor.bcdCDC = 0x1001;                           		  //USBЭ��汾��

    Descriptor_Set.Call_Management_Function_Descriptor.bFunctionLength = sizeof(Call_Management_Function_Descriptor_t);
    Descriptor_Set.Call_Management_Function_Descriptor.bDescriptorType = 0x24;                 	  //����ӿ�����CS_INTERFACE, ���Ϊ0x24
    Descriptor_Set.Call_Management_Function_Descriptor.bDescriptorSubType = 0x01;                 //����ΪCall Management                          
    Descriptor_Set.Call_Management_Function_Descriptor.bmCapabilities = 0x00;       		      //�豸�Լ�������call management                               
    Descriptor_Set.Call_Management_Function_Descriptor.bDataInterface = 0x00;                     //û��������ӿ���Ϊcall management             

    Descriptor_Set.ACM_Functional_Descriptor.bFunctionLength = sizeof(ACM_Functional_Descriptor_t);
    Descriptor_Set.ACM_Functional_Descriptor.bDescriptorType = 0x24;                 	  		  //����ӿ�����CS_INTERFACE, ���Ϊ0x24
    Descriptor_Set.ACM_Functional_Descriptor.bDescriptorSubType = 0x02;                 		  //����ΪACM                         
    Descriptor_Set.ACM_Functional_Descriptor.bmCapabilities = 0x02;       		      			  //D1 - 1(֧��Set_Line_Coding, Set_Control_Line_Status, Get_Line_Coding, Serial_State)                              

    Descriptor_Set.Union_Functional_Descriptor.bFunctionLength = sizeof(Union_Functional_Descriptor_t);
    Descriptor_Set.Union_Functional_Descriptor.bDescriptorType = 0x24;                 	  		  //����ӿ�����CS_INTERFACE, ���Ϊ0x24
    Descriptor_Set.Union_Functional_Descriptor.bDescriptorSubType = 0x06;                 		  //����ΪUnion                          
    Descriptor_Set.Union_Functional_Descriptor.MasterInterface = 0x00;       		      		  //�豸�Լ�������call management                               
    Descriptor_Set.Union_Functional_Descriptor.SlaveInterface = 0x01;                     		  //���������Ϊ1��������ӿ�  

	// �ӿ�0ʹ������˵�1
    Descriptor_Set.Endpoint0_Descriptor.bLength = sizeof(Endpoint_Descriptor_t);
    Descriptor_Set.Endpoint0_Descriptor.bDescriptorType = 0x05;
    Descriptor_Set.Endpoint0_Descriptor.bEndpointAddress = 0x81;                             	  //ʹ������˵�1
    Descriptor_Set.Endpoint0_Descriptor.bmAttributes = 0x03;                                 	  //D1~D0�˵㴫������ѡ��(�ö˵�Ϊ�ж϶˵�)
    Descriptor_Set.Endpoint0_Descriptor.wMaxPacketSize = 0x0010;                             	  //�˵��������(��������Ϊ16���ֽ�)
    Descriptor_Set.Endpoint0_Descriptor.bInterval = 0x0A;     


    Descriptor_Set.CDC_DataInterface_Descriptor.bLength = sizeof(CDC_Interface_Descriptor_t);       //�ӿ����������ݳ���
    Descriptor_Set.CDC_DataInterface_Descriptor.bDescriptorType = 0x04;                             //�ӿ��������ı��
    Descriptor_Set.CDC_DataInterface_Descriptor.bInterfaceNumber = 0x01;                            //�ýӿڵı�� Master Interface
    Descriptor_Set.CDC_DataInterface_Descriptor.bAlternateSetting = 0x00;                           //�ýӿڵı��ñ��
    Descriptor_Set.CDC_DataInterface_Descriptor.bNumEndpoints = 0x02;                               //��0�˵����Ŀ(CDCֻʹ��һ����0����˵���Ϊbulk����)
    Descriptor_Set.CDC_DataInterface_Descriptor.bInterfaceClass = 0x0A;                             //������ӿ�
    Descriptor_Set.CDC_DataInterface_Descriptor.bInterfaceSubClass = 0x00;                         
    Descriptor_Set.CDC_DataInterface_Descriptor.bInterfaceProtocol = 0x00;                          
    Descriptor_Set.CDC_DataInterface_Descriptor.iInterface = 0x00;   

    Descriptor_Set.Endpoint2IN_Descriptor.bLength = sizeof(Endpoint_Descriptor_t);
    Descriptor_Set.Endpoint2IN_Descriptor.bDescriptorType = 0x05;
    Descriptor_Set.Endpoint2IN_Descriptor.bEndpointAddress = 0x82;                             		//Ep2 IN
    Descriptor_Set.Endpoint2IN_Descriptor.bmAttributes = 0x02;   									//Bulk transfer                              
    Descriptor_Set.Endpoint2IN_Descriptor.wMaxPacketSize = 0x0040;     								//Ep2������64bytes                        
    Descriptor_Set.Endpoint2IN_Descriptor.bInterval = 0x00;                							//Bulk transfer��IntervalҪ��                    

    Descriptor_Set.Endpoint2OUT_Descriptor.bLength = sizeof(Endpoint_Descriptor_t);
    Descriptor_Set.Endpoint2OUT_Descriptor.bDescriptorType = 0x05;
    Descriptor_Set.Endpoint2OUT_Descriptor.bEndpointAddress = 0x02;                             	//Ep2 OUT
    Descriptor_Set.Endpoint2OUT_Descriptor.bmAttributes = 0x02;   									//Bulk transfer                              
    Descriptor_Set.Endpoint2OUT_Descriptor.wMaxPacketSize = 0x0040;     							//Ep2������64bytes                        
    Descriptor_Set.Endpoint2OUT_Descriptor.bInterval = 0x00;                						//Bulk transfer��IntervalҪ��                    

    LanguageIDDescriptor_Init();
}
