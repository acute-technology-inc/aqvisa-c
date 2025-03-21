#ifndef AQLAVISA_ERRORCODE
#define AQVI_ERRORCODE


//Operation succeeded without error
#define AQVI_NO_ERROR                   0

//Error: OPENRM should only be called once to establish the software connection
#define AQVI_ALREADY_OPEN               1

//Error: The main software application was not started before calling the viOPENRM
#define AQVI_APPLICATION_NOT_STARTED    2

//Error: Failed to establish the connection to software application
#define AQVI_FAILED_TO_CONNECT          3

//Error: The software connection is not valid yet, call viOpenRM first to establish the connection
#define AQVI_CONNECTION_NOT_VALID       4

//Error: The software application has been closed or re-started, need to call viCloseRM and viOpenRM to establish the connection again
#define AQVI_CONNECTION_LOST            5

//Error: Unable to send data to software application
#define AQVI_FAILED_TO_COMMUNICATE      6

//Error: There's no any data returned from the software application
#define AQVI_NO_RETURN_DATA             7

//Error: Input data buffer is NULL pointer and not valid
#define AQVI_INPUT_NULL_POINTER         8

//Error: Input data buffer size is too small
#define AQVI_DATA_BUFFER_TOO_SMALL      9

//Error: Select Application type should be done before OpenVM
#define AQVI_SEL_APP_AFTER_VM_OPENED    10

//Error: Selected invalid Application type
#define AQVI_SEL_APP_TYPE_INVALID       11

//Error: Previous Input Command Still Processing
#define AQVI_PREVIOUS_CMD_PROCESSING    12

//Error: Input parameter unknown or not supported
#define AQVI_INPUT_PARAMETER_UNKNOWN     13

//Error: Input parameter incompleted
#define AQVI_INPUT_PARAMETER_INCOMPLETED 14


//Command Error: Input command incomplete
#define AQVI_INCOMPLETE_COMMAND         1001

//Command Error: Input command requires exist SubWindow
#define AQVI_SUBWND_INVALID             1002

//Command Error: Input command ask to add new SubWindow page, but software already exceed maximum page count
#define AQVI_SUBWND_CNT_EXCEED          1003

//Command Error: Input command ignored while software busy
#define AQVI_SW_BUSY                    1004

//Command Error: Input command requires exist Logic Analyzer SubWindow
#define AQVI_LASUBWND_INVALID           1005

//Command Error: Input command requires exist Protocol Analyzer SubWindow
#define AQVI_PASUBWND_INVALID           1006

//Command Error: Input command requires exist Decode Report
#define AQVI_DECODE_REPORT_INVALID      1007

//Command Error: Input commands requires exist Timing Report
#define AQVI_TIMING_REPORT_INVALID      1008

//Command Error: Input command format error
#define AQVI_INPUT_COMMAND_FORMAT_ERROR 1009

//Command Error: Input file directory invalid
#define AQVI_INPUT_FILE_DIR_INVALID     1010

//Command Error: Sending Capture Start command while capture already running
#define AQVI_CAPTURE_ALREAY_RUNNING     1011

//Command Error: Sending Capture Stop command while capture is not running
#define AQVI_CAPTURE_NOT_RUNNING        1012

//Command Error: Input Row or Column index invalid
#define AQVI_ROW_COL_INDEX_INVALID      1013

//Command Error: Input index selection invalid
#define AQVI_SELECT_INDEX_INVALID       1014

//Command Error: Missing input parameters or not enough input parameter
#define AQVI_INPUT_PARAMETER_INVALID    1015

//Command Error: Input Timing setting file format error
#define AQVI_INPUT_TIMING_SETTING_FILE_FORMAT_ERROR 1016

//Error: Unknown Command
#define AQVI_UNKNOWN_COMMAND_ERROR      9997

//Error: Unknown Error
#define AQVI_UNKNOWN_ERROR              9998


//Error: DLL initialization not finished, please retry again later
#define AQVI_DLL_NOT_READY              9999

#endif // AQLAVISA_ERRORCODE

