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
    LanguageID_Descriptor.bDescriptorType = 0x03;               //�ַ���������0x03
    LanguageID_Descriptor.wLANGID = 0x0409;                     //��ʽӢ��ID
}


void Descriptors_Init(void)
{
    DeviceDescriptor_Init();

    Descriptor_Set.Configuration_Descriptor.bLength = sizeof(Configuration_Descriptor_t);
    Descriptor_Set.Configuration_Descriptor.bDescriptorType = 0x02;
    Descriptor_Set.Configuration_Descriptor.wTotalLength = (uint16_t)(sizeof(Configuration_Descriptor_t) + 2*sizeof(Interface_Descriptor_t) + 2*sizeof(HID_Descriptor_t) + 3*sizeof(Endpoint_Descriptor_t));
    Descriptor_Set.Configuration_Descriptor.bNumInterface = 0x02;                           //�������������������������Ľӿ�(һ������꣬һ��������)
    Descriptor_Set.Configuration_Descriptor.bConfigurationValue = 0x01;
    Descriptor_Set.Configuration_Descriptor.iConfiguration = 0x00;
    Descriptor_Set.Configuration_Descriptor.bmAttributes = 0x80;                            //USB���߹���
    Descriptor_Set.Configuration_Descriptor.bMaxPower = 200;                                //����200mA

    // Keyboard
    Descriptor_Set.Interface_Descriptor1.bLength = sizeof(Interface_Descriptor_t);           //�ӿ����������ݳ���
    Descriptor_Set.Interface_Descriptor1.bDescriptorType = 0x04;                             //�ӿ��������ı��
    Descriptor_Set.Interface_Descriptor1.bInterfaceNumber = 0x00;                            //�ýӿڵı��
    Descriptor_Set.Interface_Descriptor1.bAlternateSetting = 0x00;                           //�ýӿڵı��ñ��
    Descriptor_Set.Interface_Descriptor1.bNumEndpoints = 0x02;                               //��0�˵����Ŀ(USB���ֻ��Ҫһ���ж�����˵�, �������Ϊ1; USB����ʹ���������˵�)
    Descriptor_Set.Interface_Descriptor1.bInterfaceClass = 0x03;                             //�ýӿ�ʹ�õ�����룬HID��ı�����0x03
    Descriptor_Set.Interface_Descriptor1.bInterfaceSubClass = 0x01;                          //��������������.USB���̣�������ڸ�����,���������0x01
    Descriptor_Set.Interface_Descriptor1.bInterfaceProtocol = 0x01;                          //�������Ϊ֧���������������࣬��Э���ѡ�����ͼ��̣����̴���Ϊ0x01, ������Ϊ0x02
    Descriptor_Set.Interface_Descriptor1.iInterface = 0x00;                                  //�ýӿڵ��ַ�������ֵ(����û�У�����Ϊ0)

    // Mouse
    Descriptor_Set.Interface_Descriptor2.bLength = sizeof(Interface_Descriptor_t);           //�ӿ����������ݳ���
    Descriptor_Set.Interface_Descriptor2.bDescriptorType = 0x04;                             //�ӿ��������ı��
    Descriptor_Set.Interface_Descriptor2.bInterfaceNumber = 0x01;                            //�ýӿڵı��
    Descriptor_Set.Interface_Descriptor2.bAlternateSetting = 0x00;                           //�ýӿڵı��ñ��
    Descriptor_Set.Interface_Descriptor2.bNumEndpoints = 0x01;                               //��0�˵����Ŀ(USB���ֻ��Ҫһ���ж�����˵�, �������Ϊ1; USB����ʹ���������˵�)
    Descriptor_Set.Interface_Descriptor2.bInterfaceClass = 0x03;                             //�ýӿ�ʹ�õ�����룬HID��ı�����0x03
    Descriptor_Set.Interface_Descriptor2.bInterfaceSubClass = 0x01;                          //��������������.USB���̣�������ڸ�����,���������0x01
    Descriptor_Set.Interface_Descriptor2.bInterfaceProtocol = 0x01;                          //�������Ϊ֧���������������࣬��Э���ѡ�����ͼ��̣����̴���Ϊ0x01, ������Ϊ0x02
    Descriptor_Set.Interface_Descriptor2.iInterface = 0x00;                                  //�ýӿڵ��ַ�������ֵ(����û�У�����Ϊ0)

    // HID1 - Keyboard
    Descriptor_Set.HID_Descriptor1.bLength = sizeof(HID_Descriptor_t);
    Descriptor_Set.HID_Descriptor1.bDescriptorType = 0x21;                                   //HID�������ı��Ϊ0x21
    Descriptor_Set.HID_Descriptor1.bcdHID = 0x0110;                                          //HID1.1Э��
    Descriptor_Set.HID_Descriptor1.bCountryCode = 0x21;                                      //�豸���õĹ��Ҵ���Ϊ����, 0x21
    Descriptor_Set.HID_Descriptor1.bNumDescriptors = 0x01;                                   //�¼�����������Ŀ,ֻ��һ������������
    Descriptor_Set.HID_Descriptor1.HID_SubDescriptor[0].bDescriptorType = 0x22;              //�¼�����������Ϊ���������������Ϊ0x22
    Descriptor_Set.HID_Descriptor1.HID_SubDescriptor[0].wDescriptorLength = 53;
    
    // HID2 - Mouse
    Descriptor_Set.HID_Descriptor2.bLength = sizeof(HID_Descriptor_t);
    Descriptor_Set.HID_Descriptor2.bDescriptorType = 0x21;                                   //HID�������ı��Ϊ0x21
    Descriptor_Set.HID_Descriptor2.bcdHID = 0x0110;                                          //HID1.1Э��
    Descriptor_Set.HID_Descriptor2.bCountryCode = 0x21;                                      //�豸���õĹ��Ҵ���Ϊ����, 0x21
    Descriptor_Set.HID_Descriptor2.bNumDescriptors = 0x01;                                   //�¼�����������Ŀ,ֻ��һ������������
    Descriptor_Set.HID_Descriptor2.HID_SubDescriptor[0].bDescriptorType = 0x22;              //�¼�����������Ϊ���������������Ϊ0x22
    Descriptor_Set.HID_Descriptor2.HID_SubDescriptor[0].wDescriptorLength = 52;
    
    // Keyboard - IN endpoint
    Descriptor_Set.Endpoint_Descriptor1.bLength = sizeof(Endpoint_Descriptor_t);
    Descriptor_Set.Endpoint_Descriptor1.bDescriptorType = 0x05;
    Descriptor_Set.Endpoint_Descriptor1.bEndpointAddress = 0x81;                             //ʹ������˵�1
    Descriptor_Set.Endpoint_Descriptor1.bmAttributes = 0x03;                                 //D1~D0�˵㴫������ѡ��(�ö˵�Ϊ�ж϶˵�)
    Descriptor_Set.Endpoint_Descriptor1.wMaxPacketSize = 0x0010;                             //�˵��������(��������Ϊ16���ֽ�)
    Descriptor_Set.Endpoint_Descriptor1.bInterval = 0x0A;                                    //�˵��ѯʱ��Ϊ10��֡���ʱ�伴10ms

    // Keyboard - OUT endpoint
    Descriptor_Set.Endpoint_Descriptor2.bLength = sizeof(Endpoint_Descriptor_t);
    Descriptor_Set.Endpoint_Descriptor2.bDescriptorType = 0x05;
    Descriptor_Set.Endpoint_Descriptor2.bEndpointAddress = 0x01;                             //ʹ������˵�1
    Descriptor_Set.Endpoint_Descriptor2.bmAttributes = 0x03;                                 //D1~D0�˵㴫������ѡ��(�ö˵�Ϊ�ж϶˵�)
    Descriptor_Set.Endpoint_Descriptor2.wMaxPacketSize = 0x0010;                             //�˵��������(��������Ϊ16���ֽ�)
    Descriptor_Set.Endpoint_Descriptor2.bInterval = 0x0A;                                    //�˵��ѯʱ��Ϊ10��֡���ʱ�伴10ms

    // Mouse - IN endpoint
    Descriptor_Set.Endpoint_Descriptor3.bLength = sizeof(Endpoint_Descriptor_t);
    Descriptor_Set.Endpoint_Descriptor3.bDescriptorType = 0x05;
    Descriptor_Set.Endpoint_Descriptor3.bEndpointAddress = 0x82;                             //ʹ������˵�1
    Descriptor_Set.Endpoint_Descriptor3.bmAttributes = 0x03;                                 //D1~D0�˵㴫������ѡ��(�ö˵�Ϊ�ж϶˵�)
    Descriptor_Set.Endpoint_Descriptor3.wMaxPacketSize = 0x0010;                             //�˵��������(��������Ϊ16���ֽ�)
    Descriptor_Set.Endpoint_Descriptor3.bInterval = 0x0A;                                    //�˵��ѯʱ��Ϊ10��֡���ʱ�伴10ms

    LanguageIDDescriptor_Init();
}


