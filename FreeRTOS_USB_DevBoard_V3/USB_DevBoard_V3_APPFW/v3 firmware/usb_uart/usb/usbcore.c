#include "usb.h"


//�˵�1�����Ƿ�æ�ı�־������������������ʱ���ñ�־Ϊ�档
//���������п���ʱ���ñ�־Ϊ�١�
//uint8_t Ep1InIsBusy;
uint8_t Buffer[16] = {0};
uint8_t CDC_Buffer[64] = {0};
uint8_t CDC_DataLenReceived = 0;

//��ǰ������ֵ��ֻ�������÷�0���ú����λ
uint8_t ConfigValue = 0;

/** 
 * �Ƿ���0���ݰ��ı�־ 
 * 1.��USB���ƴ���Ĺ����У������ص����ݰ��ֽ���С��������(�˵��������)ʱ���ͻ���Ϊ���ݹ��̽���
 * 2.��������ֽ�����ʵ����Ҫ���ص��ֽ���������ʵ�ʷ��ص��ֽ����ָպ��Ƕ˵�0��С��������ʱ������Ҫ����һ������Ϊ0
 *   �����ݰ����������ݹ��̣�������NeedZeroPacket��Ϊ�Ƿ���Ҫ����һ��0�������ݰ���flag
 */
uint8_t NeedZeroPacket;
uint8_t *pSendData;
uint16_t SendLength;
Device_Request_t Device_Request;                            // Usb�����������ݰ�

// CDC property setting parameter
Line_Coding_t Line_Coding_Data;

/*
uint8_t LanguageId[4]=
{
 0x04, //���������ĳ���
 0x03, //�ַ���������
 //0x0409Ϊ��ʽӢ���ID
 0x09,
 0x04
};
*/

//�ַ�����USB��������������Unicode����
//8λС�˸�ʽ
uint8_t ManufactureStringDescriptor[18] = {
18,         //���������ĳ���Ϊ18�ֽ�
0x03,       //�ַ��������������ͱ���Ϊ0x03
0x55, 0x00, //U
0x53, 0x00, //S
0x42, 0x00, //B
0x00, 0x5f, //��
0xd1, 0x53, //��
0x7f, 0x67, //��
0x3e, 0x79, //��
0x3a, 0x53  //��
};

//�ַ�����simon.yuan's USB-CDC device����Unicode����
//8λС�˸�ʽ
uint8_t ProductStringDescriptor[56]={
56,         //���������ĳ���Ϊ56�ֽ�
0x03,       //�ַ��������������ͱ���Ϊ0x03
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


//�ַ�����2019-09-05����Unicode����
//8λС�˸�ʽ
uint8_t SerialNumberStringDescriptor[22]={
22,         //���������ĳ���Ϊ22�ֽ�
0x03,       //�ַ��������������ͱ���Ϊ0x03
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
    printf("<-----USB�����ж�.----->\r\n");
}


void UsbBusReset(void)
{
    printf("\r\n<=====USB��λ�ж�=====>\r\n");
}


void UsbEp2In(void)
{
    printf("<-----Ep2��������ж�.----->\r\n");
    D12ReadEndpointLastStatus(5);
    D12ClearBuffer();
}


// Endpoint2 received data from USB Host
void UsbEp2Out(void)
{
    uint8_t i = 0;

    printf("<-----Ep2����ж�.----->\r\n");
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
    printf("<-----Ep1��������ж�.----->\r\n");
    D12ReadEndpointLastStatus(3);
}


void UsbEp1Out(void)
{
    uint8_t Buf = 0;                                // LED status control
    printf("<-----Ep1����ж�.----->\r\n");
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
    printf("\r\n<-----Ep0��������ж�.----->\r\n");
    
    // USB�豸����UsbEp0Out�з���һ������, ������ɺ����UsbEp0In�жϷ���ʣ������
    // 1.����жϱ�־λ
    // 2.����ʣ������
    D12ReadEndpointLastStatus(1);
    UsbEp0SendData();
}


// USB host output to USB device
void UsbEp0Out(void)
{
    uint8_t Ep0OutLastStatus;

    printf("<-----Ep0����ж�.----->\r\n");

    // if Ep0 out data is setup packet, need to send ACK to make sure
    // USB host know setup packet had been received successfully
    Ep0OutLastStatus = D12ReadEndpointLastStatus(Ep0_OUT);

    // Setup packet
    if(Ep0OutLastStatus & 0x20)
    {
        printf("ͨ�Ž�����\r\n");
        D12ReadEndpointBuffer(Ep0_OUT, 16, Buffer);
        D12AckknoledgeSetup();
        D12ClearBuffer();
        USBStandardDeviceRequestCopy();

        //����bmRequestType��D6~5λɢת��D6~5λ��ʾ���������
        //0Ϊ��׼����1Ϊ������2Ϊ��������
        if((Device_Request.bmRequestType & 0x80) == 0x80)
        {
            switch((Device_Request.bmRequestType >> 5) & 0x03)
            {
                case 0:
                    printf("\r\nUSB��׼��������====>");
                    switch(Device_Request.bRequest)
                    {
                        case bRequest_GET_CONFIGURATION:
                            printf("��ȡ��������");
                            break;

                        case bRequest_GET_DESCRIPTOR:
                            printf("��ȡ��������");
                            switch((Device_Request.wValue >> 8) & 0xFF)
                            {
                                case DEVICE_DESCRIPTOR:
                                    printf("�豸������.\r\n");
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
                                    printf("����������.\r\n");
                                    pSendData = (uint8_t *)&Descriptor_Set;
                                    SendLength = Descriptor_Set.Configuration_Descriptor.wTotalLength;

                                    // ����������ݰ����ȴ���ʵ�ʷ������ݰ����ȣ���ʵ�����ݰ�����Ϊ׼
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
                                    printf("�ַ���������.\r\n");
                                    switch(Device_Request.wValue & 0xFF)
                                    {
                                        case 0:                                     //��ȡ����ID
                                            printf("Index = 0, ����ID.");
                                            pSendData = (uint8_t *)&LanguageID_Descriptor;
                                            SendLength = LanguageID_Descriptor.bLength;
                                            break;
                                         
                                        case 1:                                     //�����ַ���������ֵΪ1����������Ϊ�����ַ���
                                            printf("Index = 1, �����ַ���.");
                                            pSendData = ManufactureStringDescriptor;
                                            SendLength = ManufactureStringDescriptor[0];
                                            break;
                                        
                                        case 2:                                     //��Ʒ�ַ���������ֵΪ2����������Ϊ��Ʒ�ַ���
                                            printf("Index = 2, ��Ʒ�ַ���.");
                                            pSendData = ProductStringDescriptor;
                                            SendLength = ProductStringDescriptor[0];
                                            break;

                                        case 3:                                     //��Ʒ�ַ���������ֵΪ2����������Ϊ��Ʒ�ַ���
                                            printf("Index = 3, ���к��ַ���.");
                                            pSendData = SerialNumberStringDescriptor;
                                            SendLength = SerialNumberStringDescriptor[0];
                                            break;

                                        default:
                                            printf("δ������ַ�����.\r\n");
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

                                case REPORT_DESCRIPTOR:                             //����������
                                    printf("����������\r\n");
                                
                                    // ����Ӳ���ӿڽ������ݣ�
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
                                    printf("����������, ����������Ϊ: 0x%x\r\n", ((Device_Request.wValue >> 8) & 0xFF));
                                    break;
                            }
                            break;

                        case bRequest_GET_INTERFACE:
                            printf("��ȡ�ӿ�.\r\n");
                            break;

                        case bRequest_GET_STATUS:
                            printf("��ȡ״̬.\r\n");
                            break;

                        case bRequest_SYNCH_FRAME:
                            printf("ͬ��֡.\r\n");
                            break;

                        default:
                            printf("����δ����ı�׼��������.\r\n");
                            break;    
                    }
                    break;

                case 1:
                    printf("\r\nUSB����������.\r\n");

                    // �����������е�
                    switch(Device_Request.bRequest)
                    {
                        case bRequest_GET_LINE_CODING:
                            printf("\r\nGET_LINE_CODING����������.\r\n");
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
                            printf("\r\bRequest_SERIAL_STATE����������.\r\n");
                            SendLength = 0;
                            NeedZeroPacket = 1;
                            UsbEp0SendData();
                            break;
                    }
                    break;

                 case 2:
                    printf("\r\nUSB������������.\r\n");
                    break;

                case 3:
                    printf("\r\n����δ�������������.\r\n");
                    break;
            }
        }
        else              //(Device_Request.bmRequestType & 0x80) != 0x80(�������豸���������)
        {
            switch((Device_Request.bmRequestType >> 5) & 0x03)
            {
                case 0:
                    printf("\r\nUSB��׼�������====>");
                    switch(Device_Request.bRequest)
                    {
                        case bRequest_CLEAR_FEATURE:
                            printf("�������.\r\n");
                            break;

                        case bRequest_SET_ADDRESS:
                            printf("���õ�ַ��\r\n");
                            printf("0x%x", (Device_Request.wValue & 0xFF));
                            D12SetAddress(Device_Request.wValue & 0xFF);
                        
                            // ���õ�ַ��û�����ݷ��أ�ֱ�ӽ��뵽״̬���̣�����һ������Ϊ0�����ݰ�
                            SendLength = 0;
                            NeedZeroPacket = 1;
                            
                            // Ep0��������
                            UsbEp0SendData();
                            break;

                        case bRequest_SET_CONFIGURATION:
                            printf("��������.\r\n");
                            //ʹ�ܷ�0�˵㡣��0�˵�ֻ��������Ϊ��0�����ú����ʹ�ܡ�
                            //wValue�ĵ��ֽ�Ϊ���õ�ֵ�������ֵΪ��0������ʹ�ܷ�0�˵㡣
                            //���浱ǰ����ֵ
                            ConfigValue =  Device_Request.wValue & 0xFF;
                            D12SetEndpointEnable(ConfigValue);
                            SendLength = 0;
                            NeedZeroPacket = 1;
                            UsbEp0SendData();
                            break;

                        case bRequest_SET_DESCRIPTOR:
                            printf("����������.\r\n");
                            break;

                        case bRequest_SET_FEATURE:
                            printf("��������.\r\n");
                            break;    
                        
                        case bRequest_SET_INTERFACE:
                            printf("���ýӿ�.\r\n");
                            break;

                        default:
                            printf("����δ����ı�׼�������.\r\n");
                            break;
                    }
                    break;

                case 1:
                    printf("\r\nUSB���������====>");
                    switch(Device_Request.bRequest)
                    {
                        case bRequest_SET_IDLE:
                            printf("���ÿ���.\r\n");
                            // ֻ��Ҫ����һ��0���ȵ����ݰ�����
                            SendLength = 0;
                            NeedZeroPacket = 1;
                            UsbEp0SendData();
                            break;

                        //���������ô��ڵ����ԣ�����ʵ�ʵ����ݲ��������ù��̷�����
                        //������֮������ݹ��̷��������ﲻ�����κδ��������ݹ���
                        //��ɺ󷵻�0���ȵ�״̬����
                        case bRequest_SET_LINE_CODING:
                            printf("\r\nSET_LINE_CODING���������.\r\n");
                            break;    

                        //������û����������׶Σ�����wValue�ֶε�D0λ��ʾDTR��
                        //D1λ��ʾRTS���������ǵİ��ϵĴ��ڲ�û���������ţ����
                        //�Ը��������ǽ��Ǽ򵥵ط���һ��0���ȵ�״̬�������ݰ�����
                        case bRequest_SET_CONTROL_LINE_STATE:
                            printf("\r\nSET_CONTROL_LINE_STATE���������.\r\n");
                            SendLength = 0;
                            NeedZeroPacket = 1;
                            UsbEp0SendData();
                            break;


                        default:
                            printf("δ֪����.\r\n");
                    }
                    break;

                case 2:
                    printf("\r\nUSB�����������====>��\r\n");
                    break;
                case 3:
                    printf("\r\n����: δ�����������.\r\n");
                    break;
            }
        }
    }
    else        // Not setup packet
    {
        printf("��ͨ���ݰ�\r\n");
        D12ReadEndpointBuffer(Ep0_OUT, 16, Buffer);
        D12ClearBuffer();
    }
}


/********************************************************************
 *�������ܣ�����pData��SendLength�����ݷ��͵��˵�0�ĺ�����
 *��ڲ������ޡ�
 *��     �أ��ޡ�
 *��     ע���ޡ�
 *******************************************************************/
void UsbEp0SendData(void)
{
    // ����ͨ��Ep0���ͳ�ȥ������͵İ���Ϊ�˵�0�޶���������
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
 *  ���˵�x������
 *  (1)������Ҫѡȡ����ȡ���ݵĶ˵�.
 *  (2)��ȡָ���˵�����ݲ���ŵ���Ӧ�Ļ�����.
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
    // �������Ҫ��ѡ��˵�1(0 -> OUT, 1 -> IN)
    D12SelectEndpoint(1);                                           // USB�˵�0����
    D12WriteCommand(D12_REGISTER_AcknowledgeSetup);                 
    D12SelectEndpoint(0);                                           // USB�˵�0����������ѡ��Ķ˵�������Clear buffer�����Ӧ
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

