# Signature verification bypass
Bypasses the requirement for your callback to be in a signed driver.
# Usage
Include these 3 headers and use the `register_unsigned_callbacks` function when registering unsigned callbacks. The function is pretty much the same as `ObRegisterCallbacks`.
# Note
This works on `Windows 10 19045.5011`. I haven't tested it yet on Windows 11.
