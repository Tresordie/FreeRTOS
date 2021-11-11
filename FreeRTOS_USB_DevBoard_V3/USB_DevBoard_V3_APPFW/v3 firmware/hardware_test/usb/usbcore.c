#include "usb.h"
#include "bsp.h"


//�˵�1�����Ƿ�æ�ı�־������������������ʱ���ñ�־Ϊ�档
//���������п���ʱ���ñ�־Ϊ�١�
uint8_t Ep1InIsBusy;
uint8_t Buffer[16] = {0};

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


/*
//�ַ�����simon.yuan's USB Mouse����Unicode����
//8λС�˸�ʽ
uint8_t ProductStringDescriptor[46] = {
46,         //���������ĳ���Ϊ46�ֽ�
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
0x20, 0x00, // 
0x4d, 0x00, //M
0x6f, 0x00, //o
0x75, 0x00, //u
0x73, 0x00, //s
0x65, 0x00, //e
//ע���뽫���һ������Ķ���ɾ��
};
*/

//�ַ�����simon.yuan's Keyboard����Unicode����
//8λС�˸�ʽ
uint8_t ProductStringDescriptor[44]={
44,         //���������ĳ���Ϊ44�ֽ�
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
//�ַ�����2019-09-02����Unicode����
//8λС�˸�ʽ
uint8_t SerialNumberStringDescriptor[22] = {
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
0x32, 0x00  //2
};
*/

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
//USB�����������Ķ���
uint8_t ReportDescriptor[65] =
{
 //ÿ�п�ʼ�ĵ�һ�ֽ�Ϊ����Ŀ��ǰ׺��ǰ׺�ĸ�ʽΪ��
 //D7~D4��bTag��D3~D2��bType��D1~D0��bSize�����·ֱ��ÿ����Ŀע�͡�
 
 //����һ��ȫ�֣�bTypeΪ1����Ŀ������;ҳѡ��Ϊ��ͨ����Generic Desktop Page(0x01)
 //�����һ�ֽ����ݣ�bSizeΪ1����������ֽ����Ͳ�ע���ˣ�
 //�Լ�����bSize���жϡ�
 0x05, 0x01, // USAGE_PAGE (Generic Desktop)
 
 //����һ���ֲ���bTypeΪ2����Ŀ��˵���������ļ�����;���ڼ���
 0x09, 0x06, // USAGE (Keyboard)
 
 //����һ������Ŀ��bTypeΪ0����Ŀ�������ϣ������������0x01��ʾ
 //�ü�����һ��Ӧ�ü��ϡ�����������ǰ������;ҳ����;����Ϊ
 //��ͨ�����õļ��̡�
 0xa1, 0x01, // COLLECTION (Application)
 
 //����һ��ȫ����Ŀ��ѡ����;ҳΪ���̣�Keyboard/Keypad(0x07)��
 0x05, 0x07, //     USAGE_PAGE (Keyboard/Keypad)

 //����һ���ֲ���Ŀ��˵����;����СֵΪ0xe0��ʵ�����Ǽ�����Ctrl����
 //�������;ֵ����HID��;���в鿴��
 0x19, 0xe0, //     USAGE_MINIMUM (Keyboard LeftControl)
 
 //����һ���ֲ���Ŀ��˵����;�����ֵΪ0xe7��ʵ�����Ǽ�����GUI����
 0x29, 0xe7, //     USAGE_MAXIMUM (Keyboard Right GUI)
 
 //����һ��ȫ����Ŀ��˵�����ص����ݵ��߼�ֵ���������Ƿ��ص��������ֵ��
 //��СΪ0����Ϊ����������Bit����ʾһ�������������СΪ0�����Ϊ1��
 0x15, 0x00, //     LOGICAL_MINIMUM (0)
 
 //����һ��ȫ����Ŀ��˵���߼�ֵ���Ϊ1��
 0x25, 0x01, //     LOGICAL_MAXIMUM (1)
 
 //����һ��ȫ����Ŀ��˵�������������Ϊ�˸���
 0x95, 0x08, //     REPORT_COUNT (8)
 
 //����һ��ȫ����Ŀ��˵��ÿ��������ĳ���Ϊ1��bit��
 0x75, 0x01, //     REPORT_SIZE (1)
 
 //����һ������Ŀ��˵����8������Ϊ1bit�������������ͳ���
 //��ǰ�������ȫ����Ŀ�����壩������Ϊ���룬
 //����Ϊ��Data,Var,Abs��Data��ʾ��Щ���ݿ��Ա䶯��Var��ʾ
 //��Щ�������Ƕ����ģ�ÿ�����ʾһ����˼��Abs��ʾ����ֵ��
 //��������Ľ�����ǣ���ĳ�����ֵΪ1ʱ���ͱ�ʾ��Ӧ�ļ����¡�
 //bit0�Ͷ�Ӧ����;��Сֵ0xe0��bit7��Ӧ����;���ֵ0xe7��
 0x81, 0x02, //     INPUT (Data,Var,Abs)
 
 //����һ��ȫ����Ŀ��˵������������Ϊ1��
 0x95, 0x01, //     REPORT_COUNT (1)
 
 //����һ��ȫ����Ŀ��˵��ÿ��������ĳ���Ϊ8bit��
 0x75, 0x08, //     REPORT_SIZE (8)
 
 //����һ������Ŀ�������ã���ǰ������ȫ����Ŀ��֪������Ϊ8bit��
 //����Ϊ1������������Ϊ�����������ص�����һֱ��0����
 //���ֽ��Ǳ����ֽڣ�������OEMʹ�ã���
 0x81, 0x03, //     INPUT (Cnst,Var,Abs)
 
 //����һ��ȫ����Ŀ������λ������Ϊ6����
 0x95, 0x06, //   REPORT_COUNT (6)
 
 //����һ��ȫ����Ŀ������ÿ��λ�򳤶�Ϊ8bit��
 //��ʵ���������Ŀ��ҪҲ�ǿ��Եģ���Ϊ��ǰ���Ѿ���һ������
 //����Ϊ8bit��ȫ����Ŀ�ˡ�
 0x75, 0x08, //   REPORT_SIZE (8)
 
 //����һ��ȫ����Ŀ�������߼���СֵΪ0��
 //ͬ�ϣ��������ȫ����ĿҲ�ǿ��Բ�Ҫ�ģ���Ϊǰ���Ѿ���һ��
 //�����߼���СֵΪ0��ȫ����Ŀ�ˡ�
 0x15, 0x00, //   LOGICAL_MINIMUM (0)
 
 //����һ��ȫ����Ŀ�������߼����ֵΪ255��
 0x25, 0xFF, //   LOGICAL_MAXIMUM (255)
 
 //����һ��ȫ����Ŀ��ѡ����;ҳΪ���̡�
 //ǰ���Ѿ�ѡ�����;ҳΪ�����ˣ����Ը���Ŀ��ҪҲ���ԡ�
 0x05, 0x07, //   USAGE_PAGE (Keyboard/Keypad)
 
 //����һ���ֲ���Ŀ��������;��СֵΪ0��0��ʾû�м����£�
 0x19, 0x00, //   USAGE_MINIMUM (Reserved (no event indicated))
 
 //����һ���ֲ���Ŀ��������;���ֵΪ0x65
 0x29, 0x65, //   USAGE_MAXIMUM (Keyboard Application)
 
 //����һ������Ŀ����˵��������8bit���������������õģ�
 //����Ϊ��Data,Ary,Abs��Data˵�������ǿ��Ա�ģ�Ary˵��
 //��Щ��������һ�����飬��ÿ��8bit�����Ա�ʾĳ����ֵ��
 //������µļ�̫�ࣨ���糬�����ﶨ��ĳ��Ȼ��߼��̱����޷�
 //ɨ����������ʱ��������Щ���ݷ���ȫ1�������ƣ�����ʾ������Ч��
 //Abs��ʾ��Щֵ�Ǿ���ֵ��
 0x81, 0x00, //     INPUT (Data,Ary,Abs)

 //����Ϊ������������
 //�߼���Сֵǰ���Ѿ��ж���Ϊ0�ˣ��������ʡ�ԡ� 
 //����һ��ȫ����Ŀ��˵���߼�ֵ���Ϊ1��
 0x25, 0x01, //     LOGICAL_MAXIMUM (1)
 
 //����һ��ȫ����Ŀ��˵������������Ϊ5���� 
 0x95, 0x05, //   REPORT_COUNT (5)
 
 //����һ��ȫ����Ŀ��˵��������ĳ���Ϊ1bit��
 0x75, 0x01, //   REPORT_SIZE (1)
 
 //����һ��ȫ����Ŀ��˵��ʹ�õ���;ҳΪָʾ�ƣ�LED��
 0x05, 0x08, //   USAGE_PAGE (LEDs)
 
 //����һ���ֲ���Ŀ��˵����;��СֵΪ���ּ��̵ơ�
 0x19, 0x01, //   USAGE_MINIMUM (Num Lock)
 
 //����һ���ֲ���Ŀ��˵����;���ֵΪKana�ơ�
 0x29, 0x05, //   USAGE_MAXIMUM (Kana)
 
 //����һ������Ŀ������������ݣ���ǰ�涨���5��LED��
 0x91, 0x02, //   OUTPUT (Data,Var,Abs)
 
 //����һ��ȫ����Ŀ������λ������Ϊ1����
 0x95, 0x01, //   REPORT_COUNT (1)
 
 //����һ��ȫ����Ŀ������λ�򳤶�Ϊ3bit��
 0x75, 0x03, //   REPORT_SIZE (3)
 
 //����һ������Ŀ���������������ǰ������5bit������������Ҫ
 //3��bit���ճ�һ�ֽڡ�
 0x91, 0x03, //   OUTPUT (Cnst,Var,Abs)
 
 //�����������Ŀ�����ر�ǰ��ļ��ϡ�bSizeΪ0�����Ժ���û���ݡ�
 0xc0        // END_COLLECTION
};
//ͨ������ı����������Ķ��壬����֪�����ص����뱨�����8�ֽڡ�
//��һ�ֽڵ�8��bit������ʾ������Ƿ��£�����Shift��Alt�ȼ�����
//�ڶ��ֽ�Ϊ����ֵ��ֵΪ����0���������ڰ��ֽ���һ����ͨ����ֵ��
//���飬��û�м�����ʱ��ȫ��6���ֽ�ֵ��Ϊ0����ֻ��һ����ͨ������ʱ��
//�������ֽ��еĵ�һ�ֽ�ֵ��Ϊ�ð����ļ�ֵ������ļ�ֵ�뿴HID��
//��;���ĵ��������ж����ͨ��ͬʱ����ʱ����ͬʱ������Щ���ļ�ֵ��
//������µļ�̫�࣬���������ֽڶ�Ϊ0xFF�����ܷ���0x00����������
//����ϵͳ��Ϊ���м����Ѿ��ͷţ������ڼ�ֵ�������е��Ⱥ�˳����
//����ν�ģ�����ϵͳ�Ḻ�����Ƿ����¼����¡�����Ӧ�����ж϶˵�1
//�а�������ĸ�ʽ����ʵ�ʵļ������ݡ����⣬�����л�������һ���ֽ�
//��������棬����������LED����ġ�ֻʹ���˵�7λ����1λ�Ǳ���ֵ0��
//��ĳλ��ֵΪ1ʱ�����ʾ��Ӧ��LEDҪ����������ϵͳ�Ḻ��ͬ������
//����֮���LED����������������̣�һ������ּ��̵���ʱ����һ��
//Ҳ������������̱�����Ҫ�жϸ���LEDӦ�ú�ʱ������ֻ�ǵȴ�����
//���ͱ��������Ȼ����ݱ���ֵ��������Ӧ��LED�������ڶ˵�1����ж�
//�ж�����1�ֽڵ�������棬Ȼ�����ȡ������Ϊѧϰ���ϵ�LED�ǵ͵�ƽʱ
//������ֱ�ӷ��͵�LED�ϡ�����main�����а�������LED�Ĵ���Ͳ���Ҫ�ˡ�
///////////////////////////�������������////////////////////////////
*/

extern Device_Descriptor_t Device_Descriptor;
extern Descriptor_Set_t Descriptor_Set;
extern LanguageID_Descriptor_t LanguageID_Descriptor;

static void USBStandardDeviceRequestCopy(void);
static void D12SetAddress(uint8_t addr);
static void D12SetEndpointEnable(uint8_t Enable);

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
}


void UsbEp2Out(void)
{
    printf("<-----Ep2����ж�.----->\r\n");
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

    printf("\r\n<-----Ep0����ж�.----->\r\n");

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
                    printf("\r\nUSB��׼��������====>\r\n");
                    switch(Device_Request.bRequest)
                    {
                        case bRequest_GET_CONFIGURATION:
                            printf("��ȡ��������\r\n");
                            break;

                        case bRequest_GET_DESCRIPTOR:
                            printf("��ȡ��������\r\n");
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
                                            printf("Index = 0, ����ID.\r\n");
                                            //pSendData = LanguageId;
                                            //SendLength = LanguageId[0];
                                            pSendData = (uint8_t *)&LanguageID_Descriptor;
                                            SendLength = LanguageID_Descriptor.bLength;
                                            break;
                                         
                                        case 1:                                     //�����ַ���������ֵΪ1����������Ϊ�����ַ���
                                            printf("Index = 1, �����ַ���.\r\n");
                                            pSendData = ManufactureStringDescriptor;
                                            SendLength = ManufactureStringDescriptor[0];
                                            break;
                                        
                                        case 2:                                     //��Ʒ�ַ���������ֵΪ2����������Ϊ��Ʒ�ַ���
                                            printf("Index = 2, ��Ʒ�ַ���.\r\n");
                                            pSendData = ProductStringDescriptor;
                                            SendLength = ProductStringDescriptor[0];
                                            break;

                                        case 3:                                     //��Ʒ�ַ���������ֵΪ2����������Ϊ��Ʒ�ַ���
                                            printf("Index = 3, ���к��ַ���.\r\n");
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
                    printf("\r\nUSB��׼�������====>\r\n");
                    switch(Device_Request.bRequest)
                    {
                        case bRequest_CLEAR_FEATURE:
                            printf("�������.\r\n");
                            break;

                        case bRequest_SET_ADDRESS:
                            printf("���õ�ַ��\r\n");
                            printf("0x%x\r\n", (Device_Request.wValue & 0xFF));
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
                    printf("\r\nUSB���������====>\r\n");
                    switch(Device_Request.bRequest)
                    {
                        case bRequest_SET_IDLE:
                            printf("���ÿ���.\r\n");
                            // ֻ��Ҫ����һ��0���ȵ����ݰ�����
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
        printf("��ͨ���ݰ�");
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

