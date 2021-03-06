#ifndef _DOCAN_FUSE_H_
#define _DOCAN_FUSE_H_

#include <string>

#define FUSE_THREAD_COUNT 10
#define DOKAN_DLL L"dokan.dll"

struct fuse_config 
{
    unsigned int umask;
	unsigned int fileumask, dirumask;
	const char *fsname, *volname;
	int help;
    int debug;
	int setsignals;
};

struct fuse_session
{
	fuse_chan *ch;
};

struct fuse_chan
{
	fuse_chan():ResolvedDokanMain(NULL), ResolvedDokanUnmount(NULL), ResolvedDokanRemoveMountPoint(NULL), dokanDll(NULL) {}
	~fuse_chan();

	//This method dynamically loads DOKAN functions
	bool init();

	typedef int (__stdcall *DokanMainType)(PDOKAN_OPTIONS,PDOKAN_OPERATIONS);
	typedef BOOL (__stdcall *DokanUnmountType)(WCHAR DriveLetter);
	typedef BOOL (__stdcall *DokanRemoveMountPointType)(LPCWSTR MountPoint);
	DokanMainType ResolvedDokanMain;
	DokanUnmountType ResolvedDokanUnmount;
	DokanRemoveMountPointType ResolvedDokanRemoveMountPoint;

	std::string mountpoint;
private:
	HMODULE dokanDll;
};

struct fuse
{
	bool within_loop;
	std::auto_ptr<fuse_chan> ch;
	fuse_session sess;
	fuse_config conf;

	struct fuse_operations ops;
	void *user_data;

	fuse() : within_loop(), user_data()
	{
		memset(&conf,0,sizeof(fuse_config));
	}
};

#endif //_DOCAN_FUSE_H_
