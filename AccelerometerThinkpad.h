// http://www.ussg.iu.edu/hypermail/linux/kernel/0412.1/0974.html

struct ShockData
{
	int status;
	short int  x0;
	short int  y0;
	short int  x1;
	short int  y1;
	short int  x2;
	short int  y2;
	short int  x3;
	short int  y3;
	short int  x4;
	short int  y4;
	short int  x5;
	short int  y5;
	short int  x6;
	short int  y6;
	short int  x7;
	short int  y7;
	short int  x8;
	short int  y8;
	short int  x9;
	short int  y9;
	short int  x10;
	short int  y10;
	short int  x11;
	short int  y11;
	short int  x12;
	short int  y12;
	short int  x13;
	short int  y13;
	short int  unknown0;
	short int  unknown1;
};
class Accelerometer {
private:
	HANDLE hFile;
	ShockData AccelerometerData;
	int OffsetX;
	int OffsetY;
protected:
public:
	Accelerometer();
	~Accelerometer();
	bool GetAccelerometerData(void);
	int  Status(void);
	int  X;
	int  Y;
};
Accelerometer::Accelerometer()
{
	try
	{
		hFile = CreateFile(__T("\\\\.\\ShockMgr"), GENERIC_READ,
			FILE_SHARE_READ | FILE_SHARE_WRITE,
			NULL, OPEN_EXISTING, 0, NULL);
		if (hFile == INVALID_HANDLE_VALUE)
			throw "Failed to open ShockMgr.";
	}
	catch (...) {}
	OffsetX = 0;
	OffsetY = 0;
	if (GetAccelerometerData())
	{
		OffsetX = -X;
		OffsetY = -Y;
		GetAccelerometerData();
	}
}
Accelerometer::~Accelerometer()
{
	CloseHandle(hFile);
}
bool Accelerometer::GetAccelerometerData(void)
{
	unsigned long ulRead = 0;
	try
	{
		if (!DeviceIoControl(hFile, 0x733fc, NULL, 0, // via IOCTL(0x733fc)
			(void*)& AccelerometerData, 0x24, &ulRead, NULL))
			throw "Failed to DeviceIoControl";
		X = AccelerometerData.x0 + OffsetX;
		Y = AccelerometerData.y0 + OffsetY;
		return true;
	}
	catch (...)
	{
		return false;
	}
	return false;
}
int Accelerometer::Status(void)
{
	return AccelerometerData.status;
}
