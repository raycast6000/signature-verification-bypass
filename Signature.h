#include <ntddk.h>

typedef unsigned char BYTE;

// Signatures
const BYTE mm_sig_array[5] = { 0xBA, 0x20, 0x00, 0x00, 0x00 }; // mov edx, 0x20
const BYTE mi_sig_array[3] = { 0x48, 0x8B, 0xCE }; // mov rcx, rsi
const BYTE dt_sig_array[7] = { 0x48, 0x8B, 0x15, 0x00, 0x00, 0x00, 0x00 };

UINT64 scan_sig(BYTE* signature, size_t sig_length, UINT64 from) {
	BYTE* buffer = (BYTE*)ExAllocatePool2(POOL_FLAG_NON_PAGED | POOL_FLAG_UNINITIALIZED, sig_length, 0x69);

	if (!buffer) {
		DbgPrintEx(0, 0, "Couldn't allocate memory for the buffer.\n", 0);

		__debugbreak();
	}

	UINT64 idx = from;
	for (;;) {
		BOOLEAN equal = TRUE;

		// Uncomment this in case you're debugging this function.
		// DbgPrintEx(0, 0, "Index: 0x%p\n", idx);

		if (!memcpy(buffer, (void*)idx, sig_length)) {
			DbgPrintEx(0, 0, "Failed to copy 0x%p memory to buffer.\n", (signature + idx));
		}

		// Comparing bytes. If one pair are not equal then we skip.
		for (int i = 0; i < sig_length; i++) {
			// printf("COMPARING: 0x%hhx | 0x%hhx\n", buffer[i], signature[i]);

			if (signature[i] == 0x0)
				continue;
			if (buffer[i] == signature[i]) {
				continue;
			}
			else {
				// printf("NOT EQUALS -> Skipping..\n");
				equal = FALSE;
				break;
			}
		}

		if (equal) {
			DbgPrintEx(0, 0, "Signature found! 0x%p\n", idx);

			break;
		}

		idx++;
	}

	if (buffer)
		ExFreePoolWithTag(buffer, 0x69);

	return idx;
}