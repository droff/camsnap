package camsnap

/*
#cgo LDFLAGS: -L/home/droff/gopath/src/github.com/droff/camsnap -lcamsnap
#include "camsnap.h"
*/
import "C"
import "unsafe"

func Get(device string, width uint16, height uint16) []byte {
	deviceName := C.CString(device)
	defer C.free(unsafe.Pointer(deviceName))

	buffer := unsafe.Pointer(C.malloc(50000))
	defer C.free(buffer)

	bufSize := C.camsnap_shot(deviceName, C.ushort(width), C.ushort(height), (*C.byte)(buffer))

	var arrayptr = uintptr(buffer)
	var rawBytes = make([]byte, bufSize)

	for i := 0; i < len(rawBytes); i++ {
		rawBytes[i] = byte(*(*C.byte)(unsafe.Pointer(arrayptr)))
		arrayptr++
	}

	return rawBytes
}
