package fileutils

import (
	"io/ioutil"
	"os"
	"strings"
)

func GetFilePaths(fpath string) []string {

	paths := strings.Split(fpath,string(os.PathSeparator))
	n := len(paths)
	start := 0
	end := n

	if n>1 {

		if paths[0] == "" {

			start = 1
		}

		if paths[n-1] == "" {
			end = n-1
		}

	}

	return paths[start:end]
}

func WriteFile(fpath string,data []byte) error {
	return ioutil.WriteFile(fpath,data,0644)
}



