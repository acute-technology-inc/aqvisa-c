// AqLAVISA_Sample.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "AqLAVISA_Symbol.h"
#include "AqLAVISA_ErrorCode.h"
#include <Windows.h>

HMODULE ghModule = 0;
VISELECTAPPTYPE		_viSelectAppType=0; 
VIOPENRM			_viOpenRM = 0;
VIWRITE				_viWrite = 0;
VIREAD				_viRead = 0;
VICLOSERM			_viCloseRM = 0;
VIERRCODE			_viErrCode = 0;
VIGETCOMMANDRESULT  _viGetCommandResult=0;

//APP CMD STATUS
enum APP_STATUS{
	APP_BUSY=0,
	APP_STANDBY,
};

//APP_TYPE
enum APP_TYPE
{
    APP_TYPE_TL = 0,
    APP_TYPE_BF,
    APP_TYPE_TB,
    APP_TYPE_MS,

    APP_TYPE_MIN = APP_TYPE_TL,
    APP_TYPE_MAX = APP_TYPE_MS,
};

//VISA
enum VISA_PROC_RET
{
    PROC_OK_WOUT_RETURN = 0,
    PROC_OK_WITH_RETURN = 1,
    PROC_FAIL = 2,
    PROC_PENDING = 3,
};

const char *cAppSelectName[]={"TravelLogic 3000/4000","LA3000/BusFinder","TravelBus","MSO"};

bool SelectLAApp();
bool LoadDllAndApiEntry();
bool CreateConnectionToLA();
bool QueryInformationFromLA(int iWaitTimes);
bool CloseConnectionToLA();

int viWrite(char *szCmd);
void viRead(char *szRet, int iBufLen, bool fShowRet);
int viGetCommandResult();
int viErrCode();
int SendCommand(char *szCommand, char szRead[256]);



/*Select App
	iAppType = 
	TravelLogic(3000 & 4000): APP_TYPE_TL
	BusFinder & LA3000		: APP_TYPE_BF
	TravelBus (1000 & 2000) : APP_TYPE_TB
	MSO                     : APP_TYPE_MS
*/
int iAppType = APP_TYPE_TB;//Select Your LA




int _tmain(int argc, _TCHAR* argv[])
{
	printf("AqLAVISA sample\n");
	if (LoadDllAndApiEntry())//Load AqVisa.DLL/AqVisa64.DLL
	{
		Sleep(1000);
		if (SelectLAApp())//Select App
			Sleep(1000);
			if (CreateConnectionToLA())//Connect to LA 
			{
				Sleep(1000);
				if (QueryInformationFromLA(0))//Send Command
				{
					Sleep(100);
					if (CloseConnectionToLA())//Disconnect
					{
						FreeLibrary(ghModule);
						Sleep(2000);
						printf("\nAll finished\n");
					}
				}
			}
	}

	system("PAUSE");
	return 0;
}


bool LoadDllAndApiEntry()
{
#if defined(_WIN64)
		const char * szDLLName = "AqVISA64.dll";
#else
		const char * szDLLName = "AqVISA64.dll";
#endif
	if( ( ghModule = LoadLibrary( szDLLName ) ) != NULL )
    {
		_viSelectAppType	= (VISELECTAPPTYPE)   GetProcAddress(ghModule, "viSelectAppType");
        _viOpenRM			= (VIOPENRM)			 GetProcAddress(ghModule, "viOpenRM"); 
		_viWrite			= (VIWRITE)				GetProcAddress(ghModule, "viWrite");  
		_viRead				= (VIREAD)				GetProcAddress(ghModule, "viRead");   
		_viCloseRM			= (VICLOSERM)		 GetProcAddress(ghModule, "viCloseRM");
		_viErrCode			= (VIERRCODE)		 GetProcAddress(ghModule, "viErrCode");    
        _viGetCommandResult = (VIGETCOMMANDRESULT)GetProcAddress(ghModule, "viGetCommandResult");
    
		
		if(_viSelectAppType && _viOpenRM && _viWrite && _viRead && _viCloseRM && _viErrCode && _viGetCommandResult)
		{
			printf("DLL load successful!\n");
			Sleep(500);
			return true;
		}
	}
	printf("DLL load fail...\n");	//unable to load dll, terminate program
	return false;
}

int viWrite(char *szCmd)
{
    char szLastCmd[256];
    static int iLastErrorCode = 0;
    if (_viWrite && _viErrCode && _viGetCommandResult)
    {
        if (_viWrite(szCmd))
        {
            int iResult = PROC_PENDING;
            while (iResult == PROC_PENDING)
            {
                iResult = _viGetCommandResult();
//                bool fRet = iResult == PROC_OK_WITH_RETURN;
                int iErrorCode = 0;
                switch (iResult)
                {
                case PROC_PENDING: Sleep(500); break;//Pending 
                
				case PROC_OK_WOUT_RETURN:   
                case PROC_OK_WITH_RETURN:   
					iLastErrorCode = 0; 
					break;
                
				case PROC_FAIL:
                    iErrorCode = _viErrCode();
                    if ( (iLastErrorCode != iErrorCode || strcmp(szLastCmd,szCmd) != 0)){
						if((iErrorCode != AQVI_SW_BUSY))
							printf("NG, Error: %d (%s)\n",iErrorCode,szCmd);
					}
					
                    iLastErrorCode = iErrorCode;
                    strcpy(szLastCmd, szCmd);
                    break;
                }
			}
            return iResult;
        }
        else
        {
            int iErrorCode = viErrCode();
            printf("(Error: %d\n)",iErrorCode);
        }
    }
    else
        printf("DLL Invalid\n");
    return PROC_FAIL;
}

void viRead(char *szRet, int iBufLen, bool fShowRet)
{
    if (_viErrCode && _viRead)
    {
        int iRetry = 50;
        while (iRetry)
        {
            if (_viRead(szRet, iBufLen))
            {
                if(fShowRet)//Show Return
					printf("  Return :%s\n",szRet);
                
				break;
            }
            else
            {
                int iErrorCode = viErrCode();
                if (iErrorCode == AQVI_NO_RETURN_DATA)
                {
                    printf(" There's no any data returned from the software application.\n") ;
					Sleep(100);
                    iRetry--;
                }
                else
                {
                    printf(" ErrorCode:%d\n",iErrorCode) ;
					break;
                }
            }
        }

    }
    else
        printf("DLL Invalid\n");
}

int viGetCommandResult()
{
    if (_viGetCommandResult)
        return _viGetCommandResult();
    else
    {
        printf("DLL Invalid\n");
        return PROC_FAIL;
    }
}

int viErrCode()
{
    if (_viErrCode)
        return _viErrCode();
    else
    {
        printf("DLL Invalid\n");
        return AQVI_DLL_NOT_READY;
    }
}




bool SelectLAApp(){

	if (_viSelectAppType && _viErrCode)
	{
		if (_viSelectAppType(iAppType))
		{	
			printf("Select App: %s(OK)\n",cAppSelectName[iAppType]);	
			return true;
			
		}
		else
		{
			int iErrorCode = viErrCode();
			printf("\nSelect App Failed!\nError Code: %d\n");
			return false;
			
		}
	}
}


bool CreateConnectionToLA()
{
	const int iOpenRMRetry = 10;
	for (int iRetry = 0; iRetry < iOpenRMRetry; iRetry++)
	{
		if (_viOpenRM())
		{
			printf("\nConnected to %s!\n",cAppSelectName[iAppType]);
			return true;
		}
		else
		{
			int iErrorCode = viErrCode();
			if (iErrorCode == AQVI_DLL_NOT_READY)
			{
				Sleep(100); //the AqVISA64.dll is still under initializing, wait 100ms and retry
			}
			else
			{
				break;
			}
		}
	}
	printf("\nUnable to connected to %s!\n...", cAppSelectName[iAppType]);
	return false;
}

int SendCommand(char *szCommand, char szRead[256]){
    int iErrCode = 0;
	int iRet=0;
    while(1){
        iRet = viWrite(szCommand);
        iErrCode = viErrCode();
        if (iErrCode == AQVI_SW_BUSY)
            continue;
        if (iErrCode != AQVI_NO_ERROR)
        {
            printf("Error Occured (Error Code: %d)\n",iErrCode);
			
			break;
        }
		else
		{
			switch(iRet){
				case PROC_OK_WOUT_RETURN:   printf("OK, No Return, %s\n",szCommand);   break;
				case PROC_OK_WITH_RETURN:   printf("OK, Return, %s\n",szCommand); break;
			}
		
		}

        int iCmdResult = PROC_PENDING;
        while (iCmdResult == PROC_PENDING)
        {
            iCmdResult = viGetCommandResult();

            switch (iCmdResult)
            {
            case PROC_OK_WOUT_RETURN:
                 //Next command
                return 0;
            case PROC_OK_WITH_RETURN:
                viRead(szRead, sizeof(szRead),false);
                return 0;
            case PROC_FAIL:
                iErrCode = viErrCode();
                if (iErrCode != AQVI_SW_BUSY){
                    //break;//fError = true; //Error Break all loop
					printf("Error Occured (Error Code: %d)\n",iErrCode);
                    return iErrCode;
                }

                else //break get cmd result loop, retry sending command again
                    Sleep(3000);

                break;
            case PROC_PENDING: break;
            }
        }
        Sleep(500);

    }
}


bool QueryInformationFromLA(int iWaitTimes)//iWaitTimes = 0 for 
{
	int iErrCode = 0;
    char szRead[256] = {0};
    //QByteArray szReadArr;
		
	
	//Send Query Command
	char *szQueryCmd[] = {"*LA:CAPTURE:START","*FILE:SAVE -WAVEFILE C:\\temp\\Test_File.msw"};
	int iQueryCmdCnt = sizeof(szQueryCmd)/sizeof(char*);

	for(int iCmdIndex = 0; iCmdIndex < iQueryCmdCnt; iCmdIndex++){
		strcpy(szRead,"");
		printf("  Send Command: %s\n", szQueryCmd[iCmdIndex]);
		printf("  Processing\n");
		iErrCode = SendCommand(szQueryCmd[iCmdIndex], szRead);			
		Sleep(1000);
		if(iErrCode){
			break;
		}
		//else{
		//	system("cls");
		//}
	}
	return true;
}

bool CloseConnectionToLA()
{
	int iErrCode=0;
	if (_viCloseRM())
	{
		printf("  Close RM successful!\n");
		Sleep(500);
		return true;		
	}
	iErrCode = viErrCode();
	printf("  Unable to Close RM, error code = %d\n", iErrCode);
			
	return false;
}
