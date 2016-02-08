package camsnap

/*
#cgo LDFLAGS: -L. -lcamsnap
#include "camsnap.h"
*/
import "C"
import "unsafe"

func Get(width byte, height byte) []byte {
	buffer := C.CString("")
	defer C.free(unsafe.Pointer(buffer))

	bufSize := C.int(0)

	buffer = C.camsnap_shot(C.ushort(width), C.ushort(height), &bufSize)
	rawBytes := C.GoBytes(unsafe.Pointer(buffer), bufSize)

	return rawBytes
}
