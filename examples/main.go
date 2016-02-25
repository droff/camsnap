package main

import "github.com/droff/camsnap"
import "fmt"

func main() {
	raw := camsnap.Get("/dev/video0", 1280, 720)
	fmt.Println(raw[0:16])
}
