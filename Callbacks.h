#include <ntddk.h>

#include "DataTable.h"

int register_unsigned_callbacks(uintptr_t p_pre_callback, uintptr_t p_post_callback, OB_CALLBACK_REGISTRATION* p_callback_registration, HANDLE* p_registration) {
	uintptr_t p_data_table_pre = (uintptr_t)get_data_table();
	uintptr_t p_data_table_post = (uintptr_t)(*(uintptr_t*)p_data_table_pre);

	if (!p_data_table_pre) {
		DbgPrintEx(0, 0, "Couldn't get the pre-data table.\n");

		__debugbreak();

		return FALSE;
	}

	DWORD32 old_flag_pre = *(DWORD32*)(p_data_table_pre - 0xE8 + 0x68);
	uintptr_t old_ptr_pre = *(uintptr_t*)(p_data_table_pre - 0xB8);
	DWORD32 old_size_pre = *(DWORD32*)(p_data_table_pre - 0xA8);

	DWORD32 old_flag_post = *(DWORD32*)(p_data_table_post - 0xE8 + 0x68);
	uintptr_t old_ptr_post = *(uintptr_t*)(p_data_table_post - 0xB8);
	DWORD32 old_size_post = *(DWORD32*)(p_data_table_post - 0xA8);

	*(DWORD32*)(p_data_table_pre - 0xE8 + 0x68) = 0x00000020;
	*(uintptr_t*)(p_data_table_pre - 0xB8) = p_pre_callback;
	*(DWORD32*)(p_data_table_pre - 0xA8) = (DWORD32)0x0;

	*(DWORD32*)(p_data_table_post - 0xE8 + 0x68) = 0x00000020;
	*(uintptr_t*)(p_data_table_post - 0xB8) = p_post_callback;
	*(DWORD32*)(p_data_table_post - 0xA8) = (DWORD32)0x0;

	if (!NT_SUCCESS(ObRegisterCallbacks(p_callback_registration, p_registration))) {
		DbgPrintEx(0, 0, "Failed to register callbacks.\n");

		return FALSE;
	}

	DbgPrintEx(0, 0, "Restoring data table..\n");

	*(DWORD32*)(p_data_table_pre - 0xE8 + 0x68) = old_flag_pre;
	*(uintptr_t*)(p_data_table_pre - 0xB8) = old_ptr_pre;
	*(DWORD32*)(p_data_table_pre - 0xA8) = old_size_pre;

	*(DWORD32*)(p_data_table_post - 0xE8 + 0x68) = old_flag_post;
	*(uintptr_t*)(p_data_table_post - 0xB8) = old_ptr_post;
	*(DWORD32*)(p_data_table_post - 0xA8) = old_size_post;

	return TRUE;
}