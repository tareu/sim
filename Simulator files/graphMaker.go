package main

import (
    "fmt"
    "os"
    "strings"
//    "path/filepath"
    "strconv"
)

type datapoint struct {
    vOne float64
    vTwo float64
    ratioAvg float64
    stepsAvg float64
    cuttingType int
}

type graphProperties struct {
    vOneMax float64
    vOneMin float64
    vTwoMax float64
    vTwoMin float64
    ratioMax float64
    ratioMin float64
    stepsMax float64
    stepsMin float64
    cuttingTypeMax int
}

func check(e error) {
    if e != nil {
        panic(e)
    }
}

func scrapeData(outputFolder string) ([]datapoint, graphProperties) {
    files, err := os.ReadDir(outputFolder)
    check(err) 

    datapoints := make([]datapoint, 0)
    for _, file := range files {
        dp := datapoint{}
        // get variable values and add to the datapoint
        vs := strings.Split(file.Name(), "_") 
        for _, v := range vs {
            v1 := strings.Split(v, "=")
         //   fmt.Println(i, v1, len(v1[0]))
            if len(v1[0]) > 0 {
                if v1[0] != "cuttingType" {
                    if dp.vOne == 0 {
                        dp.vOne, _ = strconv.ParseFloat(v1[1], 64)
                    } else {
          //  fmt.Println(i, v1)  
                        dp.vTwo, _ = strconv.ParseFloat(v1[1], 64)
                    }
                } else {
                    dp.cuttingType, _ = strconv.Atoi(v1[1])
                }
            }
        }
        //get ratio and steps values and add to datapoint
        setDirSlice := []string{outputFolder, "/", file.Name()}
        setDir := strings.Join(setDirSlice,"")
        setFiles, err := os.ReadDir(setDir)
        check(err)
        coopFix := 0
        defFix := 0
        stepsList := make([]int, 0)
        for _, setFile := range setFiles {
         //   fmt.Println(setFile.Name())
            // get ratios
            setFileDirSlice := []string{outputFolder, "/", file.Name(), "/", setFile.Name(), "/ratio.csv"}
            setFileDir := strings.Join(setFileDirSlice,"")
            dat, err := os.ReadFile(setFileDir)   
            check(err)
          //  fmt.Println(setFile.Name(), string(dat))
            dataFromFile := strings.Split(string(dat), ", ")
            if dataFromFile[0] == "0" {
                defFix++
            } else {
                coopFix++
            }
            //get steps, add to steps list
            setFileDirSlice = []string{outputFolder, "/", file.Name(), "/", setFile.Name(), "/steps.csv"}
            setFileDir = strings.Join(setFileDirSlice,"")
            dat, err = os.ReadFile(setFileDir)   
            check(err)
            stepFromFile, _ := strconv.Atoi(strings.TrimSpace(string(dat)))
            stepsList = append(stepsList, stepFromFile)
        }
        dp.ratioAvg = (1.0/(float64(coopFix) + float64(defFix))) * float64(coopFix)
        stepsSum := 0
        for _, steps := range stepsList {
            stepsSum += steps
        }
        dp.stepsAvg = float64(stepsSum)/float64(len(stepsList))
        datapoints = append(datapoints, dp)
        
    }
    gp := graphProperties{}
    gp.vOneMax = datapoints[0].vOne
    gp.vTwoMax = datapoints[0].vTwo
    gp.vOneMin = datapoints[0].vOne
    gp.vTwoMin = datapoints[0].vTwo
    gp.ratioMax = datapoints[0].ratioAvg
    gp.ratioMin = datapoints[0].ratioAvg
    gp.stepsMax = datapoints[0].stepsAvg
    gp.stepsMin = datapoints[0].stepsAvg
    gp.cuttingTypeMax = datapoints[0].cuttingType
    
    for _, datP := range datapoints {
        if datP.vOne > gp.vOneMax {
            gp.vOneMax = datP.vOne
        }  
        if datP.vTwo > gp.vTwoMax {
            gp.vTwoMax = datP.vTwo
        }  
        if datP.vOne < gp.vOneMin {
            gp.vOneMin = datP.vOne
        }  
        if datP.vTwo < gp.vTwoMin {
            gp.vTwoMin = datP.vTwo
        }  
        if datP.ratioAvg > gp.ratioMax {
            gp.ratioMax = datP.ratioAvg
        }
        if datP.ratioAvg < gp.ratioMin {
            gp.ratioMin = datP.ratioAvg
        }
        if datP.stepsAvg > gp.stepsMax {
            gp.stepsMax = datP.stepsAvg
        }
        if datP.stepsAvg < gp.stepsMin {
            gp.stepsMin = datP.stepsAvg
        }
        if datP.cuttingType > gp.cuttingTypeMax {
            gp.cuttingTypeMax = datP.cuttingType
        }
    }
    return datapoints, gp
}

//type graphProperties struct {
    //vOneMax float64
    //vOneMin float64
    //vTwoMax float64
    //vTwoMin float64
    //ratioMax float64
    //ratioMin float64
    //stepsMax float64
    //stepsMin float64
    //cuttingtypemax
//}
func plotData(datapoints []datapoint, gp graphProperties) {
    //fmt.Println(gp)
    //graph with steps as colour
    for cType := 0; cType < gp.cuttingTypeMax; cType++ {
        fileName := fmt.Sprintf("plots%d.csv", cType)
        f, err := os.Create(fileName)
        check(err)
        defer f.Close()
        
        
        for _, dp := range datapoints {
            if dp.cuttingType == cType {
                s := fmt.Sprintf("%f, %f, %f, %f\n", dp.vOne, dp.vTwo, dp.ratioAvg, dp.stepsAvg )
                _ , err := f.WriteString(s)
                check(err)
            }
        }
                f.Sync()
    }    
}

func main() {
    datapoints, graphProperties := scrapeData("output")
    plotData(datapoints, graphProperties)
}
