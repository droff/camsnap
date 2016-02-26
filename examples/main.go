package main

import (
	"fmt"
	"github.com/droff/camsnap"
)

func main() {
	raw := camsnap.Get("/dev/video0", 1280, 720)
	fmt.Println(raw[0:16])
}
