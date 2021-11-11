#ifndef __DESCRIPTOR_H__
#define __DESCRIPTOR_H__

#define HID_SUB_DESCRIPTOR_MAX         1

// �������ڴ����������
#pragma pack(1)

typedef struct
{
    uint8_t bLength;                    //���������ݰ��ֽ���
    uint8_t bDescriptorType;            //����������
    uint16_t bcdUSB;                    //BCD������ʾUSB��Э��汾��
    uint8_t bDeviceClass;               //�豸�����(USB-IF����)
    uint8_t bDeviceSubClass;            //�豸�������(USB-IF����)
    uint8_t bDeviceProtocol;            //�豸Э����(��USB-IF����)
    uint8_t bMaxPacketSize0;            //�˵�0���İ���
    uint16_t idVendor;                  //VID(����ID��USB-IF����)
    uint16_t idProduct;                 //��ƷID(�����������з���)
    uint16_t bcdDevice;                 //�豸�������
    uint8_t iManufacture;               //�豸�����ַ�������
    uint8_t iProduct;                   //��Ʒ�������ַ�������
    uint8_t iSerialNumber;              //�豸���к��ַ�������
    uint8_t bNumConfigurations;         //��ǰ�ٶ�����֧�ֵ���������
}Device_Descriptor_t;

typedef struct
{
    uint8_t bLength;                    //���������ݰ��ֽ���
    uint8_t bDescriptorType;            //����������
    uint16_t wTotalLength;              //�������������ϵ������ܳ���(���ã��ӿڣ��˵��)
    uint8_t bNumInterface;              //����֧�ֵĽӿ���
    uint8_t bConfigurationValue;        //SetConfiguration()��ȡ��ǰ���õĲ���ֵ
    uint8_t iConfiguration;             //������ǰ���õ��ַ���������������ֵ
    uint8_t bmAttributes;               //����ģʽѡ��Bit4-0������D7:���߹��磬D6:�Թ��磬D5:Զ�̻���
    uint8_t bMaxPower;                  // �豸��Ҫ����������(��λ:2mA)
}Configuration_Descriptor_t;

typedef struct
{
    uint8_t bLength;                    //���������ݰ��ֽ���
    uint8_t bDescriptorType;            //����������
    uint8_t bInterfaceNumber;           //��ǰ�ӿڱ��
    uint8_t bAlternateSetting;          //ǰһ��ȷ�Ͻӿڵı��ñ��
    uint8_t bNumEndpoints;              //�ýӿ���ʹ�õĶ˵���(�������˵�0)
    uint8_t bInterfaceClass;            //�ýӿ�ʹ�õ���(USB-IF����)
    uint8_t bInterfaceSubClass;         //�ýӿ�ʹ�õ�����(USB-IF����)
    uint8_t bInterfaceProtocol;         //�ýӿ�ʹ�õ�Э��(USB-IF����)
    uint8_t iInterface;                 //�����ýӿڵ��ַ�������
}Interface_Descriptor_t;


typedef struct
{
    uint8_t bDescriptorType;            //�¼�������������
    uint16_t wDescriptorLength;         //�¼��������ĳ���
}HID_SubDescriptor_t;


typedef struct
{
    uint8_t bLength;                    //���������ݰ��ֽ���
    uint8_t bDescriptorType;            //����������(HID 0x21)
    uint16_t bcdHID;                     //HIDЭ��汾
    uint8_t bCountryCode;               //���Ҵ���
    uint8_t bNumDescriptors;            //�¼�������������
    HID_SubDescriptor_t HID_SubDescriptor[HID_SUB_DESCRIPTOR_MAX];
}HID_Descriptor_t;


typedef struct
{
    uint8_t bLength;                    //���������ݰ��ֽ���
    uint8_t bDescriptorType;            //����������(Endpoint 0x05)
    uint8_t bEndpointAddress;           //�ö˵��ַ
    uint8_t bmAttributes;               //�ö˵�����
    uint16_t wMaxPacketSize;             //�ö˵�֧�ֵ���������
    uint8_t bInterval;                  //�˵�Ĳ�ѯʱ��
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
    uint8_t bLength;                    //���������ݰ��ֽ���
    uint8_t bDescriptorType;            //����������(Endpoint 0x05)
    uint16_t wLANGID;   //�ö˵��ַ
}LanguageID_Descriptor_t;


#pragma pack()


void Descriptors_Init(void);


#endif


