#ifndef __DESCRIPTOR_H__
#define __DESCRIPTOR_H__

#include "stm32f10x.h"

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
}CDC_Interface_Descriptor_t;


typedef struct
{
    uint8_t bFunctionLength;                    //���������ݰ��ֽ���
    uint8_t bDescriptorType;            		//����������
    uint8_t bDescriptorSubType;           		//��ǰ�ӿڱ��
    uint16_t bcdCDC;          					//USB�汾
}Header_Function_Descriptor_t;


typedef struct
{
    uint8_t bFunctionLength;                    //���������ݰ��ֽ���
    uint8_t bDescriptorType;            		//����������
    uint8_t bDescriptorSubType;           		//��ǰ�ӿڱ��
    uint8_t bmCapabilities;          			//D0 - ���ù���D1 - ���ù���ͨ��������ӿڻ���ͨ����ӿ�
    uint8_t bDataInterface;						//���ù����������ӿڱ�� 
}Call_Management_Function_Descriptor_t;


typedef struct
{												//Abstract_Control_Management
    uint8_t bFunctionLength;                    //���������ݰ��ֽ���
    uint8_t bDescriptorType;            		//����������
    uint8_t bDescriptorSubType;           		//��ǰ�ӿڱ��
    uint8_t bmCapabilities;          			//D0 - ���ù���D1 - ���ù���ͨ��������ӿڻ���ͨ����ӿ�
}ACM_Functional_Descriptor_t;


typedef struct
{												//Abstract_Control_Management
    uint8_t bFunctionLength;                    //���������ݰ��ֽ���
    uint8_t bDescriptorType;            		//����������
    uint8_t bDescriptorSubType;           		//��ǰ�ӿڱ��
    uint8_t MasterInterface;          			//���Ϊ0��CDC���ӿ�
	uint8_t SlaveInterface;						//���Ϊ1��CDC�ӽӿ�
}Union_Functional_Descriptor_t;


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
    CDC_Interface_Descriptor_t CDC_Interface_Descriptor;
	Header_Function_Descriptor_t Header_Function_Descriptor;
	Call_Management_Function_Descriptor_t Call_Management_Function_Descriptor;
	ACM_Functional_Descriptor_t ACM_Functional_Descriptor;
	Union_Functional_Descriptor_t Union_Functional_Descriptor;
	Endpoint_Descriptor_t Endpoint0_Descriptor;

	CDC_Interface_Descriptor_t CDC_DataInterface_Descriptor;
    Endpoint_Descriptor_t Endpoint2IN_Descriptor;  
	Endpoint_Descriptor_t Endpoint2OUT_Descriptor;  
}Descriptor_Set_t;


typedef struct
{
    uint8_t bLength;                    //���������ݰ��ֽ���
    uint8_t bDescriptorType;            //����������(Endpoint 0x05)
    uint16_t wLANGID;   				//�ö˵��ַ
}LanguageID_Descriptor_t;


#pragma pack()


void Descriptors_Init(void);


#endif


