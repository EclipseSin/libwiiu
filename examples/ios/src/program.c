#include "program.h"
char ex_cb(void *context);
void _entryPoint()
{
	/****************************>           Get Handles           <****************************/
	//Get a handle to coreinit.rpl
	unsigned int coreinit_handle;
	OSDynLoad_Acquire("coreinit.rpl", &coreinit_handle);
	//Get a handle to vpad.rpl */
	unsigned int vpad_handle;
	OSDynLoad_Acquire("vpad.rpl", &vpad_handle);
	/****************************>       External Prototypes       <****************************/
	//VPAD functions
	int(*VPADRead)(int controller, VPADStatus *buffer, unsigned int num, int *error);
	//OS functions
	void(*_Exit)();
	
  /* IOS functions */
  int (*IOS_Open)(unsigned char *path, int mode);
  int (*IOS_Read)(int fd, char * buffer, int length);
  
  s32 (*IOS_Ioctl)(s32 fd, s32 request, void *buffer_in, s32 bytes_in, void *buffer_io, s32 bytes_io);
  s32 (*IOS_Ioctlv)(s32 fd, s32 request, s32 InCount, s32 OutCont, void *vec);
	/****************************>             Exports             <****************************/
	//VPAD functions
	OSDynLoad_FindExport(vpad_handle, 0, "VPADRead", &VPADRead);
	//OS functions
	OSDynLoad_FindExport(coreinit_handle, 0, "_Exit", &_Exit);
	
	  OSDynLoad_FindExport(coreinit_handle, 0, "IOS_Open", &IOS_Open);
  OSDynLoad_FindExport(coreinit_handle, 0, "IOS_Read", &IOS_Read);
  
  OSDynLoad_FindExport(coreinit_handle, 0, "IOS_Ioctl", &IOS_Ioctl);
  OSDynLoad_FindExport(coreinit_handle, 0, "IOS_Ioctlv", &IOS_Ioctlv);
  
        void (*OSConsoleWrite)(u32 address, u32 length);
	  void (*OSLogRetrieve)(u32 flags, u32 dest, u32 length);
	  OSDynLoad_FindExport(coreinit_handle, 0, "OSConsoleWrite", &OSConsoleWrite);
	  OSDynLoad_FindExport(coreinit_handle, 0, "OSLogRetrieve", &OSLogRetrieve);
	    /* OS exception functions */
  char (*OSSetExceptionCallback)(unsigned char ex, char (*callback)(void*));
  /* Read the address of OSSetExceptionCallback() */
  OSDynLoad_FindExport(coreinit_handle, 0, "OSSetExceptionCallback", &OSSetExceptionCallback);
	/****************************>             Globals             <****************************/
	
	  /* Set up DSI, ISI, and program exception callbacks */
  OSSetExceptionCallback(2, &ex_cb);
  OSSetExceptionCallback(3, &ex_cb);
  OSSetExceptionCallback(6, &ex_cb);
	struct renderFlags flags;
	flags.y=0;
	flags.x=0;
	flags.a=0;
	flags.b=0;
	
	int slc = IOS_Open("/dev/slc",1);
	int mlc = IOS_Open("/dev/mlc",1);
	int wfs = IOS_Open("/dev/wfs",1);
	int di = IOS_Open("/dev/di",1);
	int atfs = IOS_Open("/dev/atfs",1);
	int mcp = IOS_Open("/dev/mcp",1);
	int crypto = IOS_Open("/dev/crypto",1);
	int drh = IOS_Open("/dev/drh",1);
	int usbdrh = IOS_Open("/dev/usb/drh",1);
	int slot0 = IOS_Open("/dev/fat",1);
	int odm = IOS_Open("/dev/odm",1);
	int ppc_kernel = IOS_Open("/dev/ppc_kernel",1);
	int ppc_app = IOS_Open("/dev/ppc_app",1);
	int dk = IOS_Open("/dev/dk",1);
	int mcp_recovery = IOS_Open("/dev/mcp_recovery",1);
	int smd = IOS_Open("/dev/smd",1);
	int ipc = IOS_Open("/dev/ipc",1);
	/****************************>            VPAD Loop            <****************************/
	int blah=0;
	/* Enter the VPAD loop */
	int error;
	VPADStatus vpad_status;
	//Read initial vpad status
	VPADRead(0, &vpad_status, 1, &error);
	unsigned int (*OSScreenPutPixelEx)(unsigned int bufferNum, unsigned int posX, unsigned int posY, u32 color);
	OSDynLoad_FindExport(coreinit_handle, 0, "OSScreenPutPixelEx", &OSScreenPutPixelEx);
		unsigned int(*OSScreenSetBufferEx)(unsigned int bufferNum, void * addr);
		OSDynLoad_FindExport(coreinit_handle, 0, "OSScreenSetBufferEx", &OSScreenSetBufferEx);
	while(1)
	{
		VPADRead(0, &vpad_status, 1, &error);

		//button A
		if (vpad_status.hold & KEY_A)
		{
			flags.y=0;
			flags.x=0;
			flags.a=1;
			flags.b=0;
			char buffer[256];
			__os_snprintf(flags.output, 1000, "/dev/slc: %d\n/dev/mlc: %d\n/dev/wfs: %d\n/dev/di: %d\n/dev/atfs: %d\n/dev/mcp: %d\n/dev/crypto: %d\n/dev/drh: %d\n/dev/usb/drh: %d\n/dev/fat: %d\n/dev/odm: %d\n/dev/ppc_kernel: %d\n/dev/ppc_app: %d\n/dev/dk: %d\n/dev/mcp_recovery: %d\n/dev/smd: %d\n/dev/ipc: %d", slc, mlc, wfs, di, atfs, mcp, crypto, drh, usbdrh, slot0,odm,ppc_kernel,ppc_app,dk,mcp_recovery,smd,ipc);
		}
	
		//button B
		else if (vpad_status.hold & KEY_B)
		{
			flags.y=0;
			flags.x=0;
			flags.a=0;
			flags.b=1;
		}
	
		//button X
		else if (vpad_status.hold & KEY_X)
		{
			flags.y=0;
			flags.x=1;
			flags.a=0;
			flags.b=0;
			__os_snprintf(flags.output, 1000, "X button pressed");
		}
	
		//button Y
		else if (vpad_status.hold & KEY_Y)
		{
			flags.y=1;
			flags.x=0;
			flags.a=0;
			flags.b=0;
			__os_snprintf(flags.output, 1000, "Y button pressed");
		}
		
		if(vpad_status.hold & KEY_HOME)
		{
		break;
		}
		render(&flags);
	}
	//WARNING: DO NOT CHANGE THIS. YOU MUST CLEAR THE FRAMEBUFFERS AND IMMEDIATELY CALL EXIT FROM THIS FUNCTION. RETURNING TO LOADER CAUSES FREEZE.
	int ii=0;
	for(ii;ii<2;ii++)
	{
		fillScreen(0,0,0,0);
		flipBuffers();
	}
	_Exit();
}

void render(struct renderFlags *flags)
{
	int i=0;
	for(i;i<2;i++)
	{
		if(flags->y)
		{
		fillScreen(0,0,0,0);
		drawString(0,0,flags->output);
		}
		if(flags->x)
		{
		fillScreen(0,0,0,0);
		drawString(0,0,flags->output);
		}
		if(flags->a)
		{
		fillScreen(0,0,0,0);
		drawString(0,0,flags->output);
		}
		if(flags->b)
		{
		fillScreen(0,0,0,0);
		drawString(0,0,flags->output);
		}
		flipBuffers();
	}
	flags->a=0;
	flags->b=0;
	flags->x=0;
	flags->y=0;
}

/* Exception callback */
char ex_cb(void *context)
{
  /* Get a handle to coreinit.rpl */
  unsigned int coreinit_handle;
  OSDynLoad_Acquire("coreinit.rpl", &coreinit_handle);

  /* Read the address of _Exit and call it */
  void (*_Exit)();
  OSDynLoad_FindExport(coreinit_handle, 0, "_Exit", &_Exit);
	//WARNING: DO NOT CHANGE THIS. YOU MUST CLEAR THE FRAMEBUFFERS AND IMMEDIATELY CALL EXIT FROM THIS FUNCTION. RETURNING TO LOADER CAUSES FREEZE.
	int ii=0;
	for(ii;ii<2;ii++)
	{
		fillScreen(0,0,0xFF,0);
		flipBuffers();
	}
	_Exit();
}