package main

/*
#cgo CFLAGS: -I./
#include "camsnap.h"
#include "camsnap.c"
*/
import "C"
import (
	"fmt"
	"io/ioutil"
	"unsafe"
)

func main() {
	buffer := C.CString("")
	defer C.free(unsafe.Pointer(buffer))

	bufSize := C.int(0)
	buffer = C.camsnap_shot(1280, 720, &bufSize)

	raw := C.GoBytes(unsafe.Pointer(buffer), bufSize)

	// save into file
	ioutil.WriteFile("./go.jpg", raw, 0660)
	fmt.Printf("%v bytes saved\n", len(raw))
}
