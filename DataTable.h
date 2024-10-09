#include <ntddk.h>
#include "Signature.h"

// Gets the address of the data table.
void* get_data_table() {
	/*
	I LITERALLY COULD DO JUST THIS BUT THE MICROSOFT COMPILER IS FUCKING RETARDED
	BYTE MmVerifyCallbackFunctionCheckFlagsSignature[5] = { 0xBA, 0x20, 0x00, 0x00, 0x00 };*/
	BYTE* MmVerifyCallbackFunctionCheckFlagsSignature = ExAllocatePoolWithTag(NonPagedPool, 5, 0x11015);

	memcpy((void*)MmVerifyCallbackFunctionCheckFlagsSignature, (void*)&mm_sig_array, 5);

	DbgPrintEx(0, 0, "Sigscanning for mov edx, 0x20 | Pre-call to MmVerifyCallbackFunctionCheckFlags\n");
	UINT64 mm_call = scan_sig(MmVerifyCallbackFunctionCheckFlagsSignature, 5, (UINT64)&ObRegisterCallbacks) + 5;

	DbgPrintEx(0, 0, "0x%llx: mov edx, 0x20\n0x%llx: call MmVerifyCallbackFunctionCheckFlags\n", mm_call - 5, mm_call);

	//												RIP      +     4-BYTE DISPLACEMENT (CALL OPERAND)
	void* MmVerifyCallbackFunctionCheckFlags = (void*)((mm_call + 5) + *(int*)(mm_call + 1));

	DbgPrintEx(0, 0, "MmVerifyCallbackFunctionCheckFlags: 0x%p\n", MmVerifyCallbackFunctionCheckFlags);

	BYTE* MiLookupDataTableEntrySignature = ExAllocatePoolWithTag(NonPagedPool, 3, 0x669);

	memcpy((void*)MiLookupDataTableEntrySignature, (void*)&mi_sig_array, 3);

	UINT64 mi_call = scan_sig(MiLookupDataTableEntrySignature, 3, (UINT64)MmVerifyCallbackFunctionCheckFlags) + 3;

	// same shit
	void* MiLookupDataTableEntry = (void*)((mi_call + 5) + *(int*)(mi_call + 1));
	DbgPrintEx(0, 0, "0x%llx: mov rcx, rsi\n0x%llx call MiLookupDataTableEntry\nMiLookupDataTableEntry: 0x%p\n", mi_call - 3, mi_call, MiLookupDataTableEntry);

	// TODO: Sigscan the data table.

	void* DataTableSignature = ExAllocatePoolWithTag(NonPagedPool, 7, 0x169);

	memcpy(DataTableSignature, (void*)&dt_sig_array, 7);

	DbgPrintEx(0, 0, "Scanning for mov rdx, MI_SYSTEM_IMAGE_STATE..\n");
	UINT64 dt_mov = scan_sig((BYTE*)DataTableSignature, 7, (UINT64)MiLookupDataTableEntry);

	DbgPrintEx(0, 0, "0x%llx: mov rdx, MI_SYSTEM_INFORMATION_STATE\n", dt_mov);

	void* DataTable = (void*)(*((UINT64*)((dt_mov + 7) + *(int*)(dt_mov + 3))));

	DbgPrintEx(0, 0, "DataTable: 0x%p\n", DataTable);

	ExFreePoolWithTag(MmVerifyCallbackFunctionCheckFlagsSignature, 0x11015);
	ExFreePoolWithTag(MiLookupDataTableEntrySignature, 0x669);
	ExFreePoolWithTag(DataTableSignature, 0x169);

	return DataTable;
}
