package rule

import (
	"archive/zip"
	"github.com/cloudhands/utils/fileutils"
	"github.com/cloudhands/utils/ruleutils"
	"os"
	"path/filepath"
	"strings"
)

type RuleFile struct {

	Engine string  `json:"engine"`
	Type string 	`json:"type"`
	Name string 	`json:"name"`
}

func NewRuleFile(engine string,ttype string,name string) *RuleFile {

	return &RuleFile{
		Engine: engine,
		Type:   ttype,
		Name:   name,
	}
}

func NewRuleFileFromPath(file string) *RuleFile {

	paths := fileutils.GetFilePaths(file)
	n := len(paths)

	name := ""
	ttype := ""
	engine := ""

	if n>3 {

		name = paths[n-1]
		ttype = paths[n-2]
		engine = paths[n-3]

	}

	return &RuleFile{
		Engine: engine,
		Type:   ttype,
		Name:   name,
	}

}


func (rfile *RuleFile)ruleCopy(rdir string,dstDir string) (err error) {

	rpath := filepath.Join(rdir,rfile.Engine,rfile.Type)


	return filepath.Walk(rpath, func(path string, info os.FileInfo, err error) error {

		if info.IsDir() {

			return nil
		}

		dpath := filepath.Join(dstDir,rfile.Engine,rfile.Type,info.Name())

		if err = fileutils.FileCopy(dpath,path);err!=nil {

			return err
		}

		return nil
	})
}


// Package some rules into zip file
func PackageRulesZip(ruleDir string,rfiles []*RuleFile) (zf string,err error) {

	zf = filepath.Join(os.TempDir(),"rules.zip")
	rdir := filepath.Join(os.TempDir(),"rules")

	fileutils.DeleteFile(zf)
	fileutils.DeleteFiles(rdir)

	/*copy all file into dst dirs*/
	for _,rf := range rfiles {

		if err = rf.ruleCopy(ruleDir,rdir); err!=nil {

			return
		}

	}

	if err = fileutils.ZipFiles(zf,rdir,fileutils.GetAllFiles(rdir),true); err !=nil {

		return
	}

	return
}

func GetRuleFilesFromZipFile(fpath string) (files []*RuleFile) {

	reader,err := zip.OpenReader(fpath)

	defer reader.Close()

	if err!=nil {

		return files
	}

	for _,f := range reader.File {

		fname := f.Name

		if strings.HasSuffix(fname,ruleutils.RuleFileExtName) {

			ruleFile := NewRuleFileFromPath(fname)

			if strings.HasPrefix(ruleFile.Name,ruleFile.Type){

				files = append(files, ruleFile)
			}
		}
	}

	return files
}

func GetRuleFilesFromRuleDir(ruleDir string) (files []*RuleFile) {

	filepath.Walk(ruleDir, func(path string, info os.FileInfo, err error) error {

		if strings.HasSuffix(path,ruleutils.RuleFileExtName){

			ruleFile := NewRuleFileFromPath(path)

			if strings.HasPrefix(ruleFile.Name,ruleFile.Type){

				files = append(files, ruleFile)
			}
		}

		return nil
	})

	return files
}


