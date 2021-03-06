﻿#ifndef IMPORTS_HPP_0589C56B_4071_48EE_B07F_312C2E392280
#define IMPORTS_HPP_0589C56B_4071_48EE_B07F_312C2E392280
#pragma once

/*
imports.hpp

импортируемые функции
*/
/*
Copyright © 1996 Eugene Roshal
Copyright © 2000 Far Group
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions
are met:
1. Redistributions of source code must retain the above copyright
   notice, this list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright
   notice, this list of conditions and the following disclaimer in the
   documentation and/or other materials provided with the distribution.
3. The name of the authors may not be used to endorse or promote products
   derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

class imports: public singleton<imports>
{
	IMPLEMENTS_SINGLETON(imports);

private:
	imports();

	const os::rtdl::module
		m_ntdll,
		m_kernel32,
		m_shell32,
		m_user32,
		m_virtdisk,
		m_rstrtmgr,
		m_netapi32,
		m_dbghelp;

	template<typename T, class Y, T stub>
	class unique_function_pointer
	{
	public:
		NONCOPYABLE(unique_function_pointer);

		explicit unique_function_pointer(const os::rtdl::module& Module): m_module(Module) {}
		operator T() const { return get_pointer(); }
		explicit operator bool() const noexcept { return get_pointer() != stub; }

	private:
		T get_pointer() const
		{
			static const T dyn_pointer = reinterpret_cast<T>(m_module.GetProcAddress(Y::get()));
			// TODO: log if nullptr
			static const T pointer = dyn_pointer? dyn_pointer : stub;
			return pointer;
		}

		const os::rtdl::module& m_module;
	};

#define DECLARE_IMPORT_FUNCTION(RETTYPE, CALLTYPE, NAME, ...)\
private: static RETTYPE CALLTYPE stub_##NAME(__VA_ARGS__);\
private: struct name_##NAME { static auto get() { return #NAME; } };\
public: const unique_function_pointer<decltype(&imports::stub_##NAME), name_##NAME, imports::stub_##NAME> NAME;

	// ntdll
	DECLARE_IMPORT_FUNCTION(NTSTATUS, NTAPI, NtQueryDirectoryFile, HANDLE FileHandle, HANDLE Event, PVOID ApcRoutine, PVOID ApcContext, PIO_STATUS_BLOCK IoStatusBlock, PVOID FileInformation, ULONG Length, FILE_INFORMATION_CLASS FileInformationClass, BOOLEAN ReturnSingleEntry, PUNICODE_STRING FileName, BOOLEAN RestartScan);
	DECLARE_IMPORT_FUNCTION(NTSTATUS, NTAPI, NtQueryInformationFile, HANDLE FileHandle, PIO_STATUS_BLOCK IoStatusBlock, PVOID FileInformation, ULONG Length, FILE_INFORMATION_CLASS FileInformationClass);
	DECLARE_IMPORT_FUNCTION(NTSTATUS, NTAPI, NtSetInformationFile, HANDLE FileHandle, PIO_STATUS_BLOCK IoStatusBlock, PVOID FileInformation, ULONG Length, FILE_INFORMATION_CLASS FileInformationClass);
	DECLARE_IMPORT_FUNCTION(NTSTATUS, NTAPI, NtQueryObject, HANDLE Handle, OBJECT_INFORMATION_CLASS ObjectInformationClass, PVOID ObjectInformation, ULONG ObjectInformationLength, PULONG ReturnLength);
	DECLARE_IMPORT_FUNCTION(NTSTATUS, NTAPI, NtOpenSymbolicLinkObject, PHANDLE LinkHandle, ACCESS_MASK DesiredAccess, POBJECT_ATTRIBUTES ObjectAttributes);
	DECLARE_IMPORT_FUNCTION(NTSTATUS, NTAPI, NtQuerySymbolicLinkObject, HANDLE LinkHandle, PUNICODE_STRING LinkTarget, PULONG ReturnedLength);
	DECLARE_IMPORT_FUNCTION(NTSTATUS, NTAPI, NtClose, HANDLE Handle);
	DECLARE_IMPORT_FUNCTION(NTSTATUS, NTAPI, RtlGetLastNtStatus);
	DECLARE_IMPORT_FUNCTION(NTSTATUS, NTAPI, RtlNtStatusToDosError, NTSTATUS Status);

	// kernel32
	DECLARE_IMPORT_FUNCTION(BOOL, WINAPI, GetConsoleKeyboardLayoutNameW, LPWSTR Buffer);
	DECLARE_IMPORT_FUNCTION(BOOLEAN, WINAPI, CreateSymbolicLinkW, LPCWSTR SymlinkFileName, LPCWSTR TargetFileName, DWORD Flags);
	DECLARE_IMPORT_FUNCTION(HANDLE, WINAPI, FindFirstFileNameW, LPCWSTR FileName, DWORD Flags, LPDWORD StringLength, LPWSTR LinkName);
	DECLARE_IMPORT_FUNCTION(BOOL, WINAPI, FindNextFileNameW, HANDLE FindStream, LPDWORD StringLength, PWCHAR LinkName);
	DECLARE_IMPORT_FUNCTION(HANDLE, WINAPI, FindFirstStreamW, LPCWSTR FileName, STREAM_INFO_LEVELS InfoLevel, LPVOID FindStreamData, DWORD Flags);
	DECLARE_IMPORT_FUNCTION(BOOL, WINAPI, FindNextStreamW, HANDLE FindStream, LPVOID FindStreamData);
	DECLARE_IMPORT_FUNCTION(DWORD, WINAPI, GetFinalPathNameByHandleW, HANDLE file, LPWSTR FilePath, DWORD FilePathSize, DWORD Flags);
	DECLARE_IMPORT_FUNCTION(BOOL, WINAPI, GetVolumePathNamesForVolumeNameW, LPCWSTR VolumeName, LPWSTR VolumePathNames, DWORD BufferLength, PDWORD ReturnLength);
	DECLARE_IMPORT_FUNCTION(BOOL, WINAPI, GetPhysicallyInstalledSystemMemory, PULONGLONG TotalMemoryInKilobytes);
	DECLARE_IMPORT_FUNCTION(BOOL, WINAPI, HeapSetInformation, HANDLE HeapHandle, HEAP_INFORMATION_CLASS HeapInformationClass, PVOID HeapInformation, SIZE_T HeapInformationLength);
	DECLARE_IMPORT_FUNCTION(BOOL, WINAPI, IsWow64Process, HANDLE Process, PBOOL Wow64Process);
	DECLARE_IMPORT_FUNCTION(BOOL, WINAPI, GetNamedPipeServerProcessId, HANDLE Pipe, PULONG ServerProcessId);
	DECLARE_IMPORT_FUNCTION(BOOL, WINAPI, CancelSynchronousIo, HANDLE Thread);
	DECLARE_IMPORT_FUNCTION(BOOL, WINAPI, SetConsoleKeyShortcuts, BOOL Set, BYTE ReserveKeys, LPVOID AppKeys, DWORD NumAppKeys);
	DECLARE_IMPORT_FUNCTION(BOOL, WINAPI, GetConsoleScreenBufferInfoEx, HANDLE ConsoleOutput, PCONSOLE_SCREEN_BUFFER_INFOEX ConsoleScreenBufferInfoEx);
	DECLARE_IMPORT_FUNCTION(BOOL, WINAPI, QueryFullProcessImageNameW, HANDLE Process, DWORD Flags, LPWSTR ExeName, PDWORD Size);
	DECLARE_IMPORT_FUNCTION(BOOL, WINAPI, TzSpecificLocalTimeToSystemTime, const TIME_ZONE_INFORMATION* TimeZoneInformation, const SYSTEMTIME* LocalTime, LPSYSTEMTIME UniversalTime);
	DECLARE_IMPORT_FUNCTION(PVOID, WINAPI, AddVectoredExceptionHandler, ULONG First, PVECTORED_EXCEPTION_HANDLER Handler);
	DECLARE_IMPORT_FUNCTION(ULONG, WINAPI, RemoveVectoredExceptionHandler, PVOID Handler);

	// shell32
	DECLARE_IMPORT_FUNCTION(HRESULT, STDAPICALLTYPE, SHCreateAssociationRegistration, REFIID riid, void** ppv);

	// user32
	DECLARE_IMPORT_FUNCTION(HPOWERNOTIFY, WINAPI, RegisterPowerSettingNotification, HANDLE hRecipient, LPCGUID PowerSettingGuid, DWORD Flags);
	DECLARE_IMPORT_FUNCTION(BOOL, WINAPI, UnregisterPowerSettingNotification, HPOWERNOTIFY Handle);

	// virtdisk
	DECLARE_IMPORT_FUNCTION(DWORD, WINAPI, GetStorageDependencyInformation, HANDLE ObjectHandle, GET_STORAGE_DEPENDENCY_FLAG Flags, ULONG StorageDependencyInfoSize, PSTORAGE_DEPENDENCY_INFO StorageDependencyInfo, PULONG SizeUsed);
	DECLARE_IMPORT_FUNCTION(DWORD, WINAPI, OpenVirtualDisk, PVIRTUAL_STORAGE_TYPE VirtualStorageType, PCWSTR Path, VIRTUAL_DISK_ACCESS_MASK VirtualDiskAccessMask, OPEN_VIRTUAL_DISK_FLAG Flags, POPEN_VIRTUAL_DISK_PARAMETERS Parameters, PHANDLE Handle);
	DECLARE_IMPORT_FUNCTION(DWORD, WINAPI, DetachVirtualDisk, HANDLE VirtualDiskHandle, DETACH_VIRTUAL_DISK_FLAG Flags, ULONG ProviderSpecificFlags);

	// rstrtmgr
	DECLARE_IMPORT_FUNCTION(DWORD, WINAPI, RmStartSession, DWORD *SessionHandle, DWORD SessionFlags, WCHAR strSessionKey[]);
	DECLARE_IMPORT_FUNCTION(DWORD, WINAPI, RmEndSession, DWORD dwSessionHandle);
	DECLARE_IMPORT_FUNCTION(DWORD, WINAPI, RmRegisterResources, DWORD dwSessionHandle, UINT nFiles, LPCWSTR rgsFilenames[], UINT nApplications, RM_UNIQUE_PROCESS rgApplications[], UINT nServices, LPCWSTR rgsServiceNames[]);
	DECLARE_IMPORT_FUNCTION(DWORD, WINAPI, RmGetList, DWORD dwSessionHandle, UINT *pnProcInfoNeeded, UINT *pnProcInfo, RM_PROCESS_INFO rgAffectedApps[], LPDWORD lpdwRebootReasons);

	// netapi32
	DECLARE_IMPORT_FUNCTION(NET_API_STATUS, NET_API_FUNCTION, NetDfsGetInfo, LPWSTR path, LPWSTR reserved_serv, LPWSTR reserved_share, DWORD level, LPBYTE *buff);

	// dbghelp
	DECLARE_IMPORT_FUNCTION(BOOL, WINAPI, MiniDumpWriteDump, HANDLE Process, DWORD ProcessId, HANDLE File, MINIDUMP_TYPE DumpType, PMINIDUMP_EXCEPTION_INFORMATION ExceptionParam, PMINIDUMP_USER_STREAM_INFORMATION UserStreamParam, PMINIDUMP_CALLBACK_INFORMATION CallbackParam);
	DECLARE_IMPORT_FUNCTION(BOOL, WINAPI, StackWalk64, DWORD MachineType, HANDLE Process, HANDLE Thread, LPSTACKFRAME64 StackFrame, PVOID ContextRecord, PREAD_PROCESS_MEMORY_ROUTINE64 ReadMemoryRoutine, PFUNCTION_TABLE_ACCESS_ROUTINE64 FunctionTableAccessRoutine, PGET_MODULE_BASE_ROUTINE64 GetModuleBaseRoutine, PTRANSLATE_ADDRESS_ROUTINE64 TranslateAddress);
	DECLARE_IMPORT_FUNCTION(BOOL, WINAPI, SymInitialize, HANDLE Process, PCSTR UserSearchPath, BOOL InvadeProcess);
	DECLARE_IMPORT_FUNCTION(BOOL, WINAPI, SymCleanup, HANDLE Process);
	DECLARE_IMPORT_FUNCTION(BOOL, WINAPI, SymFromAddr, HANDLE Process, DWORD64 Address, PDWORD64 Displacement, PSYMBOL_INFO Symbol);
	DECLARE_IMPORT_FUNCTION(DWORD,WINAPI, SymSetOptions, DWORD SymOptions);
	DECLARE_IMPORT_FUNCTION(BOOL, WINAPI, SymGetLineFromAddr64, HANDLE Process, DWORD64 Addr, PDWORD Displacement, PIMAGEHLP_LINE64 Line);
	DECLARE_IMPORT_FUNCTION(DWORD,WINAPI, UnDecorateSymbolName, PCSTR Name, PSTR OutputString, DWORD MaxStringLength, DWORD Flags);

#undef DECLARE_IMPORT_FUNCTION
};

#endif // IMPORTS_HPP_0589C56B_4071_48EE_B07F_312C2E392280
