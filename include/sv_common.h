#ifndef _SV_COMMON_H_
#define _SV_COMMON_H_

#ifdef __cplusplus

extern "C" {
#endif

#define USING_MEMORYLIST


extern int tempVal;



/* for ipc */   
#ifdef _MRVT_          
#elif defined Zd_VERSION
#elif defined _FORKERS_
#elif defined _DV_IDRIVE_
#elif defined _XPERTO_
    #define IPC_SWITCH
#else  
    //#define IPC_SWITCH
#endif

#define 	SV_IPC_CHANNEL_CNT   	4
#define   	IPC_IP_MAXLENTH    		32  //IP����

#define COMMON_NAME_LEN 256
#define COMMON_CMD_LEN 256

/* Micro for jason revised 426 storage module */
#define DISKFLAG    1     // 1: ssd + 2sd      0: 4sd


#define SV_MAX(a, b) ((a) > (b) ?(a):(b))
#define SV_MIN(a, b) ((a) < (b) ?(a):(b))

typedef unsigned char           sv_u8;
typedef unsigned short          sv_u16;
typedef unsigned int            sv_u32;
typedef signed char             sv_s8;
typedef short                   sv_s16;
typedef int                     sv_s32;
typedef unsigned long long      sv_u64;
typedef long long               sv_s64;
typedef char                    sv_char;
typedef char*                   sv_pchar;
typedef float                   sv_float;
typedef double                  sv_double;
typedef void                    sv_void;
typedef unsigned long           sv_size_t;

typedef unsigned char           SV_U8;
typedef unsigned short          SV_U16;
typedef unsigned int            SV_U32;
typedef signed char             SV_S8;
typedef short                   SV_S16;
typedef int                     SV_S32;
typedef unsigned long long      SV_U64;
typedef long long               SV_S64;
typedef char                    SV_CHAR;
typedef char*                   SV_PCHAR;
typedef float                   SV_FLOAT;
typedef double                  SV_DOUBLE;
typedef void                    SV_VOID;
typedef unsigned long           SV_SIZE_T;


#define sv_null          0L
#define sv_success       0
#define sv_failure       (-1) 

typedef enum
{
    sv_false = 0,
    sv_true  = 1
}
sv_bool;

typedef enum
{
    SV_FALSE = 0,
    SV_TRUE  = 1
}SV_BOOL;


#define SV_NULL          0L
#define SV_NULL_PTR      0L
#define SV_SUCCESS       0
#define SV_FAILURE       (-1)

#define ERROR_CODE_DEVICE 2
#define ERROR_CODE_LEVEL  0

typedef enum
{
	ERROR_CODE_MEDIA,
	ERROR_CODE_STORAGE,
	ERROR_CODE_NETWORK,
	ERROR_CODE_RFID,
	ERROR_CODE_MCU,
	ERROR_CODE_MAINCONTROL,
	ERROR_CODE_RECORD,
	ERROR_CODE_CLIP,
	ERROR_CODE_UPDATE,
}ERRORCODE_MODULE_E;

typedef enum
{
	CLIP_JSON_ERROR,
	CLIP_UPLOAD_ERROR
}ERRORCODE_CLIP_E;

typedef enum
{
	RFID_AUTH_SUCCESS,
	RFID_AUTH_ERROR,
}ERRORCODE_RFID_E;

typedef enum
{
	RECORD_NORMALL,
	RECORD_ELECLOCKOPEN,
	RECORD_FSERROR,
	RECORD_ALARMRECORDFULL,
	RECORD_RMALRECORDFULL,
	RECORD_NOCAMERA,
	RECORD_NOTOPEN
}ERRORCODE_RECOED_E;

typedef enum
{
	NETWORK_OLINE,
	NETWORK_OFFLINE,
	NETWORK_4G_SUCCESS,
	NETWORK_4G_ERROR,
	NETWORK_WIFI_SUCCESS,
	NETWORK_WIFI_ERROR
}ERRORCODE_NETWORK_E;

typedef enum
{
	UPDATE_SUCCESS,
	UPDATE_GETDATA_ERROR,
	UPDATE_CHECKMD5_ERROR,
	UPDATE_DATASIZE_ERROR,
	UPDATE_INQUIRY_TIMEOUT,
	UPDATE_INQUIRY_REJECT,
	UPDATE_INQUIRY_NOINPREVIEM=10
}ERRORCODE_UPDATE_E;



//  module 0~99 levle 0~9 code 0~999
#define SV_CREATE_ERRORCODE(LEVEL, MODULE, CODE){\
	 (SV_U32)(ERROR_CODE_DEVICE * 1000000 + MODULE * 10000 + LEVEL * 1000 + CODE)\
}

#define SV_CHECK(expr) \
do{ \
    if (!(expr)) { \
        printf("\nCHECK failed at:\n  >File name: %s\n  >Function : %s\n  >Line No. : %d\n  >Condition: %s\n", \
                __FILE__,__FUNCTION__, __LINE__, #expr); \
        return (-1); \
    } \
}while(0)

#define SV_FUN_CHECK(express,name)\
		do{\
			if (SV_SUCCESS != express)\
			{\
				printf("%s failed at %s: LINE: %d !\n", name, __FUNCTION__, __LINE__);\
				return SV_FAILURE;\
			}\
		}while(0)

#ifdef __cplusplus
}
#endif

#define UPDATE_ERRORCODE_PATH "/etc/update_errorcode"
#define UPDATE_INQUIRE_TAG "inquire"

/*****������ؽṹ��******/

#define UID_PATH "/etc/DeviceUid"
#define SV_DVR_CHN_NUM 	16
#define SV_DVR_CONFIG_FILE "/etc/sv_xml.xml"
#define SV_DVR_ALARM_NUM 8

#define SV_DVR_LICENSE_DISABLE_P		".:?&|*/%_<>"

// jason define for tp2824 detected mode
typedef struct
{
	SV_U32 Tp2824Picsize;
	SV_U32 Tp2824FrameRate;
}SV_DVR_Tp2824DeteMode_S;

typedef struct
{
	SV_DVR_Tp2824DeteMode_S Tp2824ModeChn[4];
}SV_DVR_Tp2824DeteMode_MainChn_S;


//jpgģʽ��Ƶ�ϴ����
typedef enum
{
	JPG_INTERVAL_5S,  //���5s
	JPG_INTERVAL_3S,  //���3s
	JPG_INTERVAL_1S,  //���1s
	JPG_INTERVAL_MIN  //���ݱ���������������ϴ�
	
}SV_DVR_JPG_INTERVAL_E;

//��Ƶ��ز���������ȡֵ�ο�sv_dvr_media.h 
typedef struct
{
    SV_U32 u32BitRate;              
    SV_U32 u32FrameRate;
	SV_U32 u32PicSize;
}SV_DVR_VIDEOQUALITY_S;

typedef struct
{
    SV_DVR_VIDEOQUALITY_S modeChn[4];
	SV_U32 u32SubBitRate;
    SV_U32 u32SubFrameRate;
	SV_U32 u32SubPicSize;
	SV_U8  u8JpgInterval;  // jpg��ʱ����
	SV_BOOL bAudio;
}SV_DVR_VIDEOQUALITY_MainChn_S; // jason define

//����¼��ͨ��������ȡֵ�ο�sv_dvr_media.h 
typedef struct
{
    SV_U8 u8RecChn;
	SV_U8 u8AdioChn;
}SV_DVR_RECORDCHN_S;

//¼����ز���������ȡֵ�ο�sv_dvr_media.h
typedef struct
{
    SV_BOOL bPowerOnRec;          //�Ǵ򿪿���¼��
    SV_BOOL bCyclicRec;           //�Ƿ��ѭ��¼��
    SV_BOOL bAlarmRec;            //�Ƿ�򿪱���¼��
	SV_BOOL bALarmRecLock;        //�Ƿ���������¼�񲻸���
    SV_U32  u32PreRecTime;        //Ԥ¼��ʱ��
    SV_U32  u32FileTime;          //�ļ�ʱ��
    SV_U8 u8MotionSensitivity;    //�ƶ����������
    SV_U8 u8GsensorSensitivity;   //gsensor������ 0:�� 1:�� 2:��  ����������ò���Ƶ��
    SV_U8 u8FileType;             //¼���ļ�����
	SV_U8 u8AlarmFilterTime;      //��������ʱ��
	SV_BOOL bGsensorAdvance;
}SV_DVR_RECORD_INFO_S;

//����ͷ��ز���������ȡֵ�ο�sv_dvr_media.h
typedef struct
{
	SV_U8 u8Brightness;
	SV_U8 u8Contrast;
	SV_U8 u8Saturation;
	SV_U8 u8Hue;
}SV_DVR_CAMERA_S;


typedef struct
{
	SV_DVR_CAMERA_S sChnParm[SV_DVR_CHN_NUM];
	SV_U8 MirrorH; // jason revised: ˮƽ�ʹ�ֱ����
	SV_U8 MirrorV;
}SV_DVR_CAMERA_PARAM_S;

//��Ƶ��ز���������ȡֵ�ο�sv_dvr_media.h
typedef struct
{
	SV_U8 u8AudioOutChn;      //��Ƶ���ͨ��
	SV_U8 u8Volume;           //������С
}SV_DVR_AUDIOOUT_CHN_S;

//�������ò˵��˳�ʱ�� ������ʱ�� ���ǲ˵�ͼ���ʱ��
typedef struct
{
	SV_U8 u8MenuOn;
}SV_DVR_MenuOn_Time_S;

typedef struct
{
	SV_BOOL bLcdDisplay;
	SV_BOOL bLcdHeat;
}SV_DVR_Mirror_S;


//��ʾ����ʽ��ز���
typedef struct
{
	SV_BOOL bModeloop;      //ʹ��ѭ���л�
	SV_BOOL	bIpcMode;
	SV_U8 u8HDFormat;
	SV_U8 u8GuiDisplay; // �������ļ�����֮����Ҫ�޸������ֵ: 0��Ϊ�ķָ� 12��Ϊchn1ȫ��ģʽ
	SV_U8 u8SDFormat;
}SV_DVR_SYSFORMAT_S;

//ͨ����
typedef struct
{
	SV_CHAR szCamerName[SV_DVR_CHN_NUM][64];
}SV_DVR_CAMERANAME_S;

//��������Ŷ�Ӧ����ת����ת��
typedef enum
{
	ALARM_CHN_1,
	ALARM_CHN_2,
	ALARM_CHN_3,
	ALARM_CHN_4,
	ALARM_CHN_REVERSE, //Alarm 5
	ALARM_CHN_BRAKE,   //Alarm 6
	ALARM_CHN_LEFT,    //Alarm 7
	ALARM_CHN_RIGHT,   //Alarm 8
	ALARM_CHN_COUNT
}ALARM_CHN_E;

//�Ƿ���ʾosd����
typedef struct
{
	SV_BOOL bTime;
	SV_BOOL bVideoLoss;
	SV_BOOL bChannelName;
	SV_BOOL bAlarmStatus;
	SV_BOOL bLicenseNo;
	SV_BOOL bSpeed;
}SV_DVR_OSDSTATUS_S;

//�ٶ���ز���
typedef struct
{
	SV_BOOL bAlarmEnable;
	SV_U8 u8SpeedSrc;
	SV_U8 u8SpeedUnit;
	SV_U16 u16OverSpeed;
}SV_DVR_SPEED_S;

//�û�Ȩ����ز���
typedef struct{
	SV_CHAR szUserName[32];
	SV_CHAR szPwd[32];
	SV_CHAR szGuestName[32];
	SV_CHAR szGuestPwd[32];
	SV_BOOL bLock;
}SV_DVR_USERSETUP_S;

//�豸id��ز���
typedef struct{
	SV_CHAR szLicenseNo[128];
	SV_CHAR szDeviceID[128];
	SV_CHAR szPathNo[128];
}SV_DVR_DEVICE_S;


//时区相关参数
typedef struct{
	SV_S8 s8TimeZone;    		//时区,小时
	SV_U8 u8TimeZoneMin;		//时区分钟
	SV_U8 u8Format;      		//时间格式，范围[0,1],0:YYYYMMDD 1: MMDDYYYY
	SV_BOOL bFormatAble; 		//开启格式化显示时间12小时制标�?
	SV_BOOL bGpsSync;    		//是否打开gps同步
	SV_BOOL bNtpSync;    		//是否打开ntp同步�?
	SV_CHAR szNtpServer[128];   //ntp服务器地址
}SV_DVR_TIMEZONE_S;


//acc��ز���
typedef struct{
	SV_U16 u16ShutVol;
	SV_U16 u16AccDelay;
}SV_DVR_ACC_S;

//�����������
typedef struct{
	SV_U8 u8AlarmOut1;
	SV_U8 u8AlarmOut2;
}SV_DVR_ALARMOUT_S;

//�����������
typedef struct{
	SV_BOOL bOutBuzz;
	SV_BOOL bOutVol1;
	SV_BOOL bOutVol2;
	SV_U16  u16DispChn;
}SV_DVR_ALARNIN_S;

//��������߼�����
#if 0
typedef struct{
	SV_U16  u16TrigLevel; // ԭDVR��ѡ��high/low/off����
	SV_U16  u16Delay;
   	SV_U8	U8Priority;
	SV_BOOL bRec;
}SV_DVR_ALARMSETUP_S;
#endif
typedef struct{
	SV_U8 u16TrigLevel; // jason revise
	SV_U16  u16Delay;
   	SV_U8	U8Priority;
	SV_BOOL bRec;
}SV_DVR_ALARMSETUP_S;


typedef struct
{
	SV_S32 s32x;
	SV_S32 s32y;
}SV_DVR_2DPOINT_S;

//��������
typedef struct
{
	SV_BOOL bEnableCursor;
	SV_DVR_2DPOINT_S stPointLeftUp;
	SV_DVR_2DPOINT_S stPointRightUp;
	SV_DVR_2DPOINT_S stPointLeftDown;
	SV_DVR_2DPOINT_S stPointRightDown;
}SV_DVR_CURSOR_s;

typedef struct{
	SV_DVR_ALARNIN_S stAlarm[SV_DVR_ALARM_NUM];
}SV_DVR_ALARMARRY_S;

typedef struct{
	SV_DVR_ALARMSETUP_S stAlarmSetup[SV_DVR_ALARM_NUM];
}SV_DVR_ALARMSETUPARRY_S;

typedef struct{
	SV_DVR_CURSOR_s stCursorSetup[SV_DVR_ALARM_NUM];
}SV_DVR_ALARMCURSORARRY_S;

//��ʱ¼�����
typedef struct tag_timing{
	SV_BOOL bTimerEnable;
    SV_BOOL bDayFlag[7];
    SV_U32 u32TimingRecStartTime;
    SV_U32 u32TimingRecEndTime;
}SV_DVR_TIMINGRECINFO_S;

typedef struct tag_timerinfo{
    SV_DVR_TIMINGRECINFO_S stTimingRecInfo[4];
}SV_DVR_TIMERINFO_S;

//lan�ڲ���
typedef struct {
    SV_BOOL bDhcp;
	SV_CHAR szIpAddr[19];
	SV_CHAR szMaskAddr[19];
	SV_CHAR szGateWayAddr[19];
	SV_CHAR szMacAddr[19];
}SV_DVR_LANSETUPINFO_S;

//wifi ����
typedef struct
{
	SV_BOOL bWifi; // wifi�Ƿ�����־
    SV_BOOL bDhcp; // Dhcp�Ƿ�����־
	SV_CHAR szIpAddr[19];
	SV_CHAR szMaskAddr[19];
	SV_CHAR szGateWayAddr[19];
	SV_CHAR szSSId[64];
	SV_CHAR szPassWd[64];
	SV_S32  s32AuthMode;
	SV_S32  s32NetworkType;
	SV_S32  s32EncrypType;
	SV_S32  s32DefaultKeyID;
	SV_BOOL bHiddenSsid;
	SV_BOOL bWifiSelectEnable;
	SV_U8   u8WifiSelectThres;
	//Ap setup
	SV_BOOL	bApEnable;   /* ����Ҳ���Ǻ������������ĺ��弰���� �������������ϴ��ļ��й� */
	SV_BOOL bAccessInternet; // Ap Internet���ذ�ť
	SV_CHAR szApSSID[64];  
    SV_CHAR szApWPAPSK[64]; 
	SV_U8	u8ApFrequency;
}SV_DVR_WIFISETUP_S;

//cellular ����
typedef struct
{
	SV_BOOL bCellular;	
	SV_CHAR szApn[64];
	SV_CHAR szAccNumber[64];
	SV_CHAR szUserName[64];
	SV_CHAR szPassWd[64];
}SV_DVR_CELLULARSETUP_S;

//�绰��ز���
typedef struct
{
	SV_BOOL bSms;	
	SV_BOOL bPhone;
	SV_CHAR szPhoneNum[16];
}SV_DVR_CELLULARSMSPHONE;

//������ip����
typedef struct{
	SV_CHAR szServerIP[32];
	SV_U16  u16Port;
	SV_CHAR szServerName1[128];
	SV_BOOL bEnableName1;
	SV_CHAR szServerName2[128];
	SV_BOOL bEnableName2;
	SV_CHAR szServerName3[128];
	SV_BOOL bEnableName3;
}SV_DVR_SERVERSETUP_S;

typedef enum
{
	SERVER_LAN = 0,
	SERVER_WIFI,
	SERVER_CELLULAR,
	SERVER_BUTT
}NETWORKTYPE;

typedef struct tag_SERVERSETUP{
    SV_DVR_SERVERSETUP_S stSetupArry[SERVER_BUTT];
}SV_DVR_SERVERSETUPArry_S;

//ftp
typedef struct{
	SV_CHAR szServerIP[32];
	SV_CHAR szUserName[32];
	SV_CHAR szPassword[32];
	SV_U16  u16Port;
	SV_BOOL bEnableFTP;
	SV_BOOL bUploadNormalFile;
	SV_BOOL bCellar;
}SV_DVR_FTPSETUP_S;

typedef struct{
	SV_CHAR szDevName[32];
	SV_CHAR szAvailable[32];
	SV_CHAR szTotal[32];
	SV_CHAR szBlock[4][32];
}SV_DVR_DEVINFO_S;

typedef struct tag_DVR_DEVINFO{
    SV_DVR_DEVINFO_S stDevInfoArry[5];
}SV_DVR_DEVINFOArry_S;


//
typedef struct
{
    SV_U8 u8MirrChn;
	SV_U8 u8FilpChn;
}SV_DVR_MIRRCHN_S;

//����������
typedef struct
{
    SV_BOOL bSitch;
	SV_BOOL bPowerOnSwitch;
	SV_U16 u16Interval;
}SV_DVR_BUZZERSETUP_S;

//���Բ���
typedef struct
{
	SV_U16 u8Language;
}SV_DVR_LANGUAGE_S;

// dimmer����  jason add
typedef struct{
	SV_U8 dimLevel;
	SV_U8 dimValDay;
	SV_U8 dimValNight;
}SV_DIMMER_PARAS;

typedef struct
{
	SV_U8 u8Year;
	SV_U8 u8Month;          
	SV_U8 u8WeekNum;  // �ڼ���      
	SV_U8 u8WeekDay;  // ��һ�ܵĵڼ���
	SV_U8 u8Hour;
	SV_U8 u8Minute;
	SV_U8 u8Second;
}SV_DVR_DSTWEEKTIME_S;

typedef struct
{
	SV_U8 u8Year;
	SV_U8 u8Month;
	SV_U8 u8Day;
	SV_U8 u8Hour;
	SV_U8 u8Minute;
	SV_U8 u8Second;
}SV_DVR_DATETIME_S;

//����ʱ����
typedef struct
{
	SV_BOOL bEnable;  // ����ʱ�Ƿ�����־
	SV_U8 	u8Offest; // 0: one hour  1: two hour
	SV_U8 	u8Mode;  // 0: week mode   1: date mode
	SV_DVR_DSTWEEKTIME_S stWeekTimeStart;
	SV_DVR_DSTWEEKTIME_S stWeekTimeEnd;
	SV_DVR_DATETIME_S stDateTimeStart;
	SV_DVR_DATETIME_S stDateTimeEnd;
}SV_DVR_DST_S;

#if 0

typedef struct
{
	SV_DVR_POINT_S stAccThreshold;
	SV_DVR_POINT_S stGyroThreshold;
	SV_S32 s32CompassThreshold;
}SV_DVR_GFORCE_S;
#endif

typedef struct
{
	SV_S32 s32x;
	SV_S32 s32y;
	SV_S32 s32z;
}SV_DVR_POINT_S;

typedef enum
{
	DIRCTION_FORWARD_X = 1,
	DIRCTION_FORWARD_Y,
	DIRCTION_FORWARD_Z,
	DIRCTION_REVERSE_X,
	DIRCTION_REVERSE_Y,
	DIRCTION_REVERSE_Z,
	DIRCTION_BUTT
}SV_DVR_DIRCTION_TYPE;

typedef struct
{
	SV_S32 s32Threshold;  //mg  DPS
	SV_S32 s32Duration;   //ms  MS
	SV_S32 s32Offset;	  //mg  DPS
}SV_DVR_AXIS_SETTING;

//groce У׼����
typedef struct
{
	SV_DVR_POINT_S stAccOffset;
	SV_DVR_AXIS_SETTING stAcceleration;
	SV_DVR_AXIS_SETTING stDeceleration;
	SV_DVR_AXIS_SETTING stTurnAcc;
	SV_DVR_AXIS_SETTING stTurnGyro;
	SV_DVR_AXIS_SETTING stImpact;
	SV_U8 u8ImpactFilter;
	SV_U8 u8Forward;               //dvrǰ������ȡֵSV_DVR_DIRCTION_TYPE
	SV_U8 u8Left;				   //dvr��װ������ȡֵSV_DVR_DIRCTION_TYPE
	SV_S32 s32SamplingFreHz;
}SV_DVR_GFORCE_S;

//��ʱ��������
typedef struct
{
	SV_BOOL bEnable;                //ʹ�ܿ���  0:�ر� 1:��
	SV_U8 u8WeekDay;                //���ڼ�   [0~6] ��Ӧ ���յ�����
	SV_U32 u32Time;                 //����     [0~1440] ��Ӧ 0~24Сʱ�ķ���
}SV_DVR_REBOOT_S;

typedef enum
{
	FORKERS_DELAY_0S,
	FORKERS_DELAY_15S,
	FORKERS_DELAY_30S,
	FORKERS_DELAY_60S,
	FORKERS_DELAY_120S,
	FORKERS_DELAY_180S,
	FORKERS_DELAY_BUTT
}SV_DVR_FORKERS_DELAY;

typedef enum
{
	FORKERS_ALARMDUATAION_15S,
	FORKERS_ALARMDUATAION_30S,
	FORKERS_ALARMDUATAION_60S,
	FORKERS_ALARMDUATAION_120S,
	FORKERS_ALARMDUATAION_180S,
	FORKERS_ALARMDUATAION_INFINITE,
	FORKERS_ALARMDUATAION_BUTT
}SV_DVR_FORKERS_ALARMDUATAION;

typedef enum
{
	FORKERS_ALARMOUT_LOWLEVEL,
	FORKERS_ALARMOUT_HIGHLEVEL,
	FORKERS_ALARMOUT_BUTT
}SV_DVR_ALARMOUTLEVEL;

typedef enum
{
	FORKERS_BUZZERTIME_0,
	FORKERS_BUZZERTIME_15,
	FORKERS_BUZZERTIME_30,
	FORKERS_BUZZERTIME_60,
	FORKERS_BUZZERTIME_INFINITE,
	FORKERS_BUZZERTIME_BUTT
}SV_DVR_BUZZERTIMES;


typedef struct
{
	SV_U8 u8DelayAlarm;                 //开机后x秒后开始报�?�?sec/15sec/30sec/1min/2min/3min）SV_DVR_FORKERS_DELAY
	SV_U8 u8AlarmDuration;              //报警持续时间 �?5sec/30sec/1min/2min/3min/Infinite）SV_DVR_FORKERS_DELAY
	SV_U8 u8DefaultAlarmOut1;			//Alarm out1 默认�?SV_DVR_ALARMOUTLEVEL
	SV_U8 u8DefaultAlarmOut2;			//Alarm out2 默认�?SV_DVR_ALARMOUTLEVEL
	SV_BOOL bBuzzer;                    //蜂鸣器开关控制DVR机子
	SV_BOOL bBuzzerButton;				//蜂鸣器开关控制线控器
	SV_BOOL bSendAlarm;					//是否发送RFID报警信息到服务器�?
	SV_BOOL bExternAlarm;				//�ⲿ������־
	SV_U8 u8BuzzerTimes;				//开机蜂鸣次�?�?/15/30/60/Infinite�?
}SV_DVR_FORKERS;

typedef struct 
{
	SV_U32 			u32BitRate[SV_IPC_CHANNEL_CNT];              
    SV_U32 			u32FrameRate[SV_IPC_CHANNEL_CNT];
	SV_U32 			u32PicSize[SV_IPC_CHANNEL_CNT];
	SV_U32 			u32SubBitRate[SV_IPC_CHANNEL_CNT];
    SV_U32 			u32SubFrameRate[SV_IPC_CHANNEL_CNT];
	SV_U32 			u32SubPicSize[SV_IPC_CHANNEL_CNT];
    SV_CHAR         szIpcIp[SV_IPC_CHANNEL_CNT][IPC_IP_MAXLENTH]; 
	SV_CHAR         szHostIp[IPC_IP_MAXLENTH]; 
	SV_BOOL 		bFilter;
	SV_BOOL 		bAutoAdd;
	SV_CHAR         szUsrName[SV_IPC_CHANNEL_CNT][IPC_IP_MAXLENTH]; 
	SV_CHAR 		szPassWord[SV_IPC_CHANNEL_CNT][IPC_IP_MAXLENTH]; 
}
SV_DVR_IPC_LIST;

typedef struct
{
	SV_U32  u32Leftoffset;
	SV_U32  u32RightOffset;
	SV_U32  u32UpOffset;
	SV_U32  u32DownOffset;
}SV_DVR_CORRECTPOSITION; // ��Ļƫ�ƵĲ���

typedef struct{
	SV_BOOL bEnableUplaod;
	SV_BOOL bUploadNormalFile;
	SV_BOOL bCellar;
}SV_DVR_UPLOADSETUP_S;


/*****end******/


#endif
