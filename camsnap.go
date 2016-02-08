package camsnap

/*
#cgo LDFLAGS: -L. -lcamsnap
#include "camsnap.h"
*/
import "C"
import "unsafe"

func Get(device string, width uint16, height uint16) []byte {
	deviceName := C.CString(device)
	defer C.free(unsafe.Pointer(deviceName))

	buffer := C.CString("")
	defer C.free(unsafe.Pointer(buffer))

	bufSize := C.int(0)

	buffer = C.camsnap_shot(deviceName, C.ushort(width), C.ushort(height), &bufSize)
	rawBytes := C.GoBytes(unsafe.Pointer(buffer), bufSize)

	return rawBytes
}
