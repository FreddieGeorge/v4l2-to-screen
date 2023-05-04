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
#define   	IPC_IP_MAXLENTH    		32  //IP长度

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

/*****配置相关结构体******/

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


//jpg模式视频上传间隔
typedef enum
{
	JPG_INTERVAL_5S,  //间隔5s
	JPG_INTERVAL_3S,  //间隔3s
	JPG_INTERVAL_1S,  //间隔1s
	JPG_INTERVAL_MIN  //根据编码器能力，最快上传
	
}SV_DVR_JPG_INTERVAL_E;

//视频相关参数，具体取值参考sv_dvr_media.h 
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
	SV_U8  u8JpgInterval;  // jpg的时间间隔
	SV_BOOL bAudio;
}SV_DVR_VIDEOQUALITY_MainChn_S; // jason define

//设置录像通道，具体取值参考sv_dvr_media.h 
typedef struct
{
    SV_U8 u8RecChn;
	SV_U8 u8AdioChn;
}SV_DVR_RECORDCHN_S;

//录像相关参数，具体取值参考sv_dvr_media.h
typedef struct
{
    SV_BOOL bPowerOnRec;          //是打开开机录像
    SV_BOOL bCyclicRec;           //是否打开循环录像
    SV_BOOL bAlarmRec;            //是否打开报警录像
	SV_BOOL bALarmRecLock;        //是否锁定报警录像不覆盖
    SV_U32  u32PreRecTime;        //预录像时间
    SV_U32  u32FileTime;          //文件时长
    SV_U8 u8MotionSensitivity;    //移动侦测灵敏度
    SV_U8 u8GsensorSensitivity;   //gsensor灵敏度 0:关 1:中 2:高  根据这个设置采样频率
    SV_U8 u8FileType;             //录像文件类型
	SV_U8 u8AlarmFilterTime;      //报警过滤时长
	SV_BOOL bGsensorAdvance;
}SV_DVR_RECORD_INFO_S;

//摄像头相关参数，具体取值参考sv_dvr_media.h
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
	SV_U8 MirrorH; // jason revised: 水平和垂直镜像
	SV_U8 MirrorV;
}SV_DVR_CAMERA_PARAM_S;

//音频相关参数，具体取值参考sv_dvr_media.h
typedef struct
{
	SV_U8 u8AudioOutChn;      //音频输出通道
	SV_U8 u8Volume;           //音量大小
}SV_DVR_AUDIOOUT_CHN_S;

//界面设置菜单退出时间 主界面时间 不是菜单图标的时间
typedef struct
{
	SV_U8 u8MenuOn;
}SV_DVR_MenuOn_Time_S;

typedef struct
{
	SV_BOOL bLcdDisplay;
	SV_BOOL bLcdHeat;
}SV_DVR_Mirror_S;


//显示屏制式相关参数
typedef struct
{
	SV_BOOL bModeloop;      //使能循环切换
	SV_BOOL	bIpcMode;
	SV_U8 u8HDFormat;
	SV_U8 u8GuiDisplay; // 从配置文件读出之后需要修改自身的值: 0变为四分割 12变为chn1全屏模式
	SV_U8 u8SDFormat;
}SV_DVR_SYSFORMAT_S;

//通道名
typedef struct
{
	SV_CHAR szCamerName[SV_DVR_CHN_NUM][64];
}SV_DVR_CAMERANAME_S;

//报警的序号对应的左转，右转等
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

//是否显示osd参数
typedef struct
{
	SV_BOOL bTime;
	SV_BOOL bVideoLoss;
	SV_BOOL bChannelName;
	SV_BOOL bAlarmStatus;
	SV_BOOL bLicenseNo;
	SV_BOOL bSpeed;
}SV_DVR_OSDSTATUS_S;

//速度相关参数
typedef struct
{
	SV_BOOL bAlarmEnable;
	SV_U8 u8SpeedSrc;
	SV_U8 u8SpeedUnit;
	SV_U16 u16OverSpeed;
}SV_DVR_SPEED_S;

//用户权限相关参数
typedef struct{
	SV_CHAR szUserName[32];
	SV_CHAR szPwd[32];
	SV_CHAR szGuestName[32];
	SV_CHAR szGuestPwd[32];
	SV_BOOL bLock;
}SV_DVR_USERSETUP_S;

//设备id相关参数
typedef struct{
	SV_CHAR szLicenseNo[128];
	SV_CHAR szDeviceID[128];
	SV_CHAR szPathNo[128];
}SV_DVR_DEVICE_S;


//鏃跺尯鐩稿叧鍙傛暟
typedef struct{
	SV_S8 s8TimeZone;    		//鏃跺尯,灏忔椂
	SV_U8 u8TimeZoneMin;		//鏃跺尯鍒嗛挓
	SV_U8 u8Format;      		//鏃堕棿鏍煎紡锛岃寖鍥碵0,1],0:YYYYMMDD 1: MMDDYYYY
	SV_BOOL bFormatAble; 		//寮�鍚牸寮忓寲鏄剧ず鏃堕棿12灏忔椂鍒舵爣蹇?
	SV_BOOL bGpsSync;    		//鏄惁鎵撳紑gps鍚屾
	SV_BOOL bNtpSync;    		//鏄惁鎵撳紑ntp鍚屾锛?
	SV_CHAR szNtpServer[128];   //ntp鏈嶅姟鍣ㄥ湴鍧�
}SV_DVR_TIMEZONE_S;


//acc相关参数
typedef struct{
	SV_U16 u16ShutVol;
	SV_U16 u16AccDelay;
}SV_DVR_ACC_S;

//报警输出参数
typedef struct{
	SV_U8 u8AlarmOut1;
	SV_U8 u8AlarmOut2;
}SV_DVR_ALARMOUT_S;

//报警输入参数
typedef struct{
	SV_BOOL bOutBuzz;
	SV_BOOL bOutVol1;
	SV_BOOL bOutVol2;
	SV_U16  u16DispChn;
}SV_DVR_ALARNIN_S;

//报警输入高级参数
#if 0
typedef struct{
	SV_U16  u16TrigLevel; // 原DVR中选择high/low/off触发
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

//倒车坐标
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

//定时录像参数
typedef struct tag_timing{
	SV_BOOL bTimerEnable;
    SV_BOOL bDayFlag[7];
    SV_U32 u32TimingRecStartTime;
    SV_U32 u32TimingRecEndTime;
}SV_DVR_TIMINGRECINFO_S;

typedef struct tag_timerinfo{
    SV_DVR_TIMINGRECINFO_S stTimingRecInfo[4];
}SV_DVR_TIMERINFO_S;

//lan口参数
typedef struct {
    SV_BOOL bDhcp;
	SV_CHAR szIpAddr[19];
	SV_CHAR szMaskAddr[19];
	SV_CHAR szGateWayAddr[19];
	SV_CHAR szMacAddr[19];
}SV_DVR_LANSETUPINFO_S;

//wifi 参数
typedef struct
{
	SV_BOOL bWifi; // wifi是否开启标志
    SV_BOOL bDhcp; // Dhcp是否开启标志
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
	SV_BOOL	bApEnable;   /* 具体也不是很清楚这个变量的含义及作用 但好像与联网上传文件有关 */
	SV_BOOL bAccessInternet; // Ap Internet开关按钮
	SV_CHAR szApSSID[64];  
    SV_CHAR szApWPAPSK[64]; 
	SV_U8	u8ApFrequency;
}SV_DVR_WIFISETUP_S;

//cellular 参数
typedef struct
{
	SV_BOOL bCellular;	
	SV_CHAR szApn[64];
	SV_CHAR szAccNumber[64];
	SV_CHAR szUserName[64];
	SV_CHAR szPassWd[64];
}SV_DVR_CELLULARSETUP_S;

//电话相关参数
typedef struct
{
	SV_BOOL bSms;	
	SV_BOOL bPhone;
	SV_CHAR szPhoneNum[16];
}SV_DVR_CELLULARSMSPHONE;

//服务器ip设置
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

//蜂鸣器参数
typedef struct
{
    SV_BOOL bSitch;
	SV_BOOL bPowerOnSwitch;
	SV_U16 u16Interval;
}SV_DVR_BUZZERSETUP_S;

//语言参数
typedef struct
{
	SV_U16 u8Language;
}SV_DVR_LANGUAGE_S;

// dimmer参数  jason add
typedef struct{
	SV_U8 dimLevel;
	SV_U8 dimValDay;
	SV_U8 dimValNight;
}SV_DIMMER_PARAS;

typedef struct
{
	SV_U8 u8Year;
	SV_U8 u8Month;          
	SV_U8 u8WeekNum;  // 第几周      
	SV_U8 u8WeekDay;  // 那一周的第几天
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

//夏令时参数
typedef struct
{
	SV_BOOL bEnable;  // 夏令时是否开启标志
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

//groce 校准参数
typedef struct
{
	SV_DVR_POINT_S stAccOffset;
	SV_DVR_AXIS_SETTING stAcceleration;
	SV_DVR_AXIS_SETTING stDeceleration;
	SV_DVR_AXIS_SETTING stTurnAcc;
	SV_DVR_AXIS_SETTING stTurnGyro;
	SV_DVR_AXIS_SETTING stImpact;
	SV_U8 u8ImpactFilter;
	SV_U8 u8Forward;               //dvr前进方向，取值SV_DVR_DIRCTION_TYPE
	SV_U8 u8Left;				   //dvr安装的左方向，取值SV_DVR_DIRCTION_TYPE
	SV_S32 s32SamplingFreHz;
}SV_DVR_GFORCE_S;

//定时重启参数
typedef struct
{
	SV_BOOL bEnable;                //使能开关  0:关闭 1:打开
	SV_U8 u8WeekDay;                //星期几   [0~6] 对应 周日到周六
	SV_U32 u32Time;                 //分钟     [0~1440] 对应 0~24小时的分钟
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
	SV_U8 u8DelayAlarm;                 //寮�鏈哄悗x绉掑悗寮�濮嬫姤璀?锛?sec/15sec/30sec/1min/2min/3min锛塖V_DVR_FORKERS_DELAY
	SV_U8 u8AlarmDuration;              //鎶ヨ鎸佺画鏃堕棿 锛?5sec/30sec/1min/2min/3min/Infinite锛塖V_DVR_FORKERS_DELAY
	SV_U8 u8DefaultAlarmOut1;			//Alarm out1 榛樿鍊?SV_DVR_ALARMOUTLEVEL
	SV_U8 u8DefaultAlarmOut2;			//Alarm out2 榛樿鍊?SV_DVR_ALARMOUTLEVEL
	SV_BOOL bBuzzer;                    //铚傞福鍣ㄥ紑鍏虫帶鍒禗VR鏈哄瓙
	SV_BOOL bBuzzerButton;				//铚傞福鍣ㄥ紑鍏虫帶鍒剁嚎鎺у櫒
	SV_BOOL bSendAlarm;					//鏄惁鍙戦�丷FID鎶ヨ淇℃伅鍒版湇鍔″櫒涓?
	SV_BOOL bExternAlarm;				//外部报警标志
	SV_U8 u8BuzzerTimes;				//寮�鏈鸿渹楦ｆ鏁?锛?/15/30/60/Infinite锛?
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
}SV_DVR_CORRECTPOSITION; // 屏幕偏移的补偿

typedef struct{
	SV_BOOL bEnableUplaod;
	SV_BOOL bUploadNormalFile;
	SV_BOOL bCellar;
}SV_DVR_UPLOADSETUP_S;


/*****end******/


#endif
