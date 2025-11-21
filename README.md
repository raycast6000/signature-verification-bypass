# Signature verification bypass
Bypasses the requirement for your callback to be in a signed driver.
# Usage
Include these 3 headers and use the `register_unsigned_callbacks` function when registering unsigned callbacks. The function is pretty much the same as `ObRegisterCallbacks`. Here's an example:
```c
UNICODE_STRING altitude;
	RtlInitUnicodeString(&altitude, L"320000");

	OB_OPERATION_REGISTRATION opRegs[1] = {0};

	OB_OPERATION_REGISTRATION operation_registration;
	operation_registration.ObjectType = PsProcessType;
	operation_registration.Operations = OB_OPERATION_HANDLE_CREATE;
	operation_registration.PreOperation = PobPreOperationCallback;
	operation_registration.PostOperation = PobPostOperationCallback;
	
	opRegs[0] = operation_registration;

	OB_CALLBACK_REGISTRATION callback_registration;
	callback_registration.RegistrationContext = DeviceObject->DeviceExtension;
	callback_registration.Version = OB_FLT_REGISTRATION_VERSION;
	callback_registration.OperationRegistrationCount = 1;
	callback_registration.Altitude = altitude;
	callback_registration.OperationRegistration = opRegs;

	if (!register_unsigned_callbacks((uintptr_t)PobPreOperationCallback, (uintptr_t)PobPostOperationCallback, &callback_registration, &hRegistration)) {
		DbgPrintEx(0, 0, "Failed to register unsigned callbacks.\n");

		__debugbreak();

		return STATUS_DRIVER_UNABLE_TO_LOAD;
	}
```
# Note
This works on `Windows 10 19045.5011`. I haven't tested it yet on Windows 11.
