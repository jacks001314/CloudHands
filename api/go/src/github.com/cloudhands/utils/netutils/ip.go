package netutils

import "fmt"

func IPv4Str(ip uint32) string {
	return fmt.Sprintf("%d.%d.%d.%d",byte(ip),byte(ip>>8),byte(ip>>16),byte(ip>>24))
}


