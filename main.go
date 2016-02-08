package camsnap

/*
#cgo CFLAGS: -I./
#include "camsnap.h"
#include "camsnap.c"
*/
import "C"
import (
	"fmt"
	"image/jpeg"
	"io"
	"io/ioutil"
	"os"
	"unsafe"
)

type CamImage struct {
	r    io.Reader
	data []byte
	size int
}

func (ci *CamImage) Read(p []byte) (n int, err error) {
	for i, b := range ci.data {
		p[i] = b
	}

	return
}

func main() {
	buffer := C.CString("")
	defer C.free(unsafe.Pointer(buffer))

	bufSize := C.int(0)
	buffer = C.camsnap_shot(1280, 720, &bufSize)
	raw := C.GoBytes(unsafe.Pointer(buffer), bufSize)

	camImage := CamImage{data: raw, size: int(bufSize)}

	//img := os.Create("./go.jpg")
	//defer img.Close()

	img, _ := jpeg.Encode(&camImage)

	// save into file
	ioutil.WriteFile("./go.jpg", img, 0660)
	fmt.Printf("%v bytes saved\n", len(raw))
}
