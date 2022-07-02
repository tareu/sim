package main

import (
    "os/exec"
    "os"
    "bufio"
    "fmt"
    "strconv"
    "regexp"
)

const yninput string = "^[yn]$"
const ininput string = "^[0-9][0-9]*$"
const flinput string = "^([0-9]*[.])?[0-9]+$"
var threadCount = 0
var maxThreads = 6

type variable struct {
    attributeName string
    kind string
    value1 float64
    value2 int
}
type variableList struct {
    attributeName string
    kind string
    floatList []float64
    intList []int
}
    
type variableSSE struct {
    attributeName string
    kind string
    floatSSE [3]float64
    intSSE [3]int
}

type simulationAttributes struct {
    variableRanges []variableSSE
    graphSize int 
    selectionIntensity float64
    benefit int
    cost int
    numReps int
    directory string
    edgeProbability float64
    fullOutput int 
    cuttingType int
}

func testInput(input string, correctExpression string) string {
    match, _ := regexp.MatchString(correctExpression, input)
    for !match {
        fmt.Println("Input not valid, valid input =", correctExpression)
        scanner := bufio.NewScanner(os.Stdin)
        scanner.Scan()
        input = scanner.Text()
        match, _ = regexp.MatchString(correctExpression, input)
    }
    return input
}

func setVariablesAndConstants() *simulationAttributes{
    attributeDictionary := map[string]string{"graphSize": "int", "selectionIntensity": "float64", "benefit": "int", "cost": "int", "numReps": "int", "directory": "string", "edgeProbability": "float64", "fullOutput": "int", "cuttingType": "int"}
    s := simulationAttributes{} 
    sp := &s
    scanner := bufio.NewScanner(os.Stdin)
    for attribute, kind := range attributeDictionary {
        cValue := ""
        input := ""
        fmt.Println(attribute)
        
        if attribute == "numReps" || attribute == "directory" || attribute == "fullOutput" {
            input = "n"
        } else {
            fmt.Println("var? y/n")
            scanner.Scan()
            input = testInput(scanner.Text(), yninput)
        }
        vchoice := input
        if input == "n" {
            fmt.Println("set const val:")
            scanner.Scan()
            if kind == "float64" {
                cValue = testInput(scanner.Text(), flinput)
            }
            if kind == "int" {
                cValue = testInput(scanner.Text(), ininput)
            }
        }
        if input == "y" {
            variableStruct := variableSSE{}
            vsp := &variableStruct
            vsp.attributeName = attribute
            vsp.kind = kind
            for {
                fmt.Println("set", attribute, "start val:")
                scanner.Scan()
                if kind == "int" {
                    input = testInput(scanner.Text(), ininput)
                    vsp.intSSE[0], _ = strconv.Atoi(input) 
                }
                if kind == "float64" {
                    input = testInput(scanner.Text(), flinput)
                    vsp.floatSSE[0], _ = strconv.ParseFloat(input, 64)
                } 
                fmt.Println("set", attribute, "step val:")
                scanner.Scan()
                input = scanner.Text()
                if kind == "int" {
                    input = testInput(scanner.Text(), ininput)
                    vsp.intSSE[1], _ = strconv.Atoi(input) 
                }
                if kind == "float64" {
                    input = testInput(scanner.Text(), flinput)
                    vsp.floatSSE[1], _ = strconv.ParseFloat(input, 64)
                } 
                fmt.Println("set", attribute, "end val:")
                scanner.Scan()
                input = scanner.Text()
                if kind == "int" {
                    input = testInput(scanner.Text(), ininput)
                    vsp.intSSE[2], _ = strconv.Atoi(input) 
                }
                if kind == "float64" {
                    input = testInput(scanner.Text(), flinput)
                    vsp.floatSSE[2], _ = strconv.ParseFloat(input, 64)
                } 

                if kind == "int" {
                    end := vsp.intSSE[2]
                    step := vsp.intSSE[1]
                    start := vsp.intSSE[0]
                    if start < end && step > 0 && step <= end {
                        break
                    }
                    
                }
                if kind == "float64" {
                    end := vsp.floatSSE[2]
                    step := vsp.floatSSE[1]
                    start := vsp.floatSSE[0]
                    if start < end && step > 0 && step < end {
                        break
                    }
                } 
                fmt.Println("Your variables go backwards or something, please take more care when entering the start, step and end values. Enter them again.")
            } 
            if kind == "int" {
                sp.variableRanges = append(sp.variableRanges, variableStruct)
            }
            if kind == "float64" {
                sp.variableRanges = append(sp.variableRanges, variableStruct)
            } 
            
            //fmt.Println(sp.variableRanges)
  
        }
        if kind == "int" && vchoice == "n"  {
            value, _ := strconv.Atoi(cValue) 
            if attribute == "graphSize" {
                sp.graphSize = value
            }
            if attribute == "benefit" {
                sp.benefit = value
            }
            if attribute == "cost" {
                sp.cost = value
            }
            if attribute == "numReps" {
                sp.numReps = value
            }
            if attribute == "fullOutput" {
                sp.fullOutput = value
            }
            if attribute == "cuttingType" {
                sp.cuttingType = value
            }
        }
        if kind == "float64" && vchoice == "n" {
            value, _ := strconv.ParseFloat(cValue, 64)
            if attribute == "selectionIntensity" {
                sp.selectionIntensity = value
            }
            if attribute == "edgeProbability" {
                sp.edgeProbability = value
            }
        } 
        if kind == "string" && vchoice == "n" {
            value := cValue 
            if attribute == "directory" {
                sp.directory = value
            }
        } 
    }
    return &s
}

func simulateGraphs(graphSize int, selectionIntensity float64, benefit int, cost int, numReps int, directory string, edgeProbability float64, fullOutput int, cuttingType int) {
    generator := exec.Command("/bin/sh", "generator.sh", strconv.Itoa(graphSize), strconv.FormatFloat(selectionIntensity, 'f', -1, 64), strconv.Itoa(benefit), strconv.Itoa(cost), "0", "0", strconv.Itoa(numReps), directory, strconv.FormatFloat(edgeProbability, 'f', -1, 64), strconv.Itoa(fullOutput), strconv.Itoa(cuttingType))
    _, err := generator.Output()
    if err != nil {
        panic(err)
    }
}
func cartN(a ...[]variable) [][]variable {
    c := 1
    for _, a := range a {
        c *= len(a)
    }
    if c == 0 {
        return nil
    }
    p := make([][]variable, c)
    b := make([]variable, c*len(a))
    n := make([]int, len(a))
    s := 0
    for i := range p {
        e := s + len(a)
        pi := b[s:e]
        p[i] = pi
        s = e
        for j, n := range n {
            pi[j] = a[j][n]
        }
        for j := len(n) - 1; j >= 0; j-- {
            n[j]++
            if n[j] < len(a[j]) {
                break
            }
            n[j] = 0
        }
    }
    return p
}
//0 1 1 1 0 1 1 0 1
func main() {
    for {
        s := setVariablesAndConstants()
        fmt.Println("Variables set, simulating graphs.")
        //fmt.Println(s.graphSize, s.selectionIntensity, s.benefit, s.cost, s.numReps, s.directory, s.edgeProbability, s.fullOutput, s.cuttingType)
 
        variableListList := make([]variableList, 0)

        for _, vr := range s.variableRanges {
            vl := variableList{}
            vlp := &vl
            vlp.kind = vr.kind
            if vlp.kind == "int" {
                start := vr.intSSE[0]
                step := vr.intSSE[1]
                end := vr.intSSE[2]
                for j := start; !(j > end); j = j + step {
                    vlp.intList = append(vlp.intList, j)
                    vlp.attributeName = vr.attributeName
                }
            }
            if vlp.kind == "float64" {
                start := vr.floatSSE[0]
                step := vr.floatSSE[1]
                end := vr.floatSSE[2]
                for j := start; !(j > end); j = j + step {
                    vlp.floatList = append(vlp.floatList, j)
                    vlp.attributeName = vr.attributeName
                }
            }
            //fmt.Println(vl)
            variableListList = append(variableListList, vl)
        }
        variableRangeRange := make([][]variable, 0)
        if len(variableListList) > 0 {
            for _, vl := range variableListList {
                variableRange := make([]variable, 0)
                if vl.kind == "int" {
                    for _, vlv := range vl.intList {
                        v := variable{attributeName: vl.attributeName, kind: vl.kind, value2: vlv}
                        variableRange = append(variableRange, v)
                    }
                } 
                if vl.kind == "float64" {
                    for _, vlv := range vl.floatList {
                        v := variable{attributeName: vl.attributeName, kind: vl.kind, value1: vlv}
                        variableRange = append(variableRange, v)
                    }
                } 
                variableRangeRange = append(variableRangeRange, variableRange)
                //fmt.Println("var range:", variableRange)
            }
        }
        combinations := cartN(variableRangeRange...) 
        for combinationNumber, c := range combinations {
            tempS := *s
            tempPS := &tempS 
            for _, v := range c {
                if v.attributeName == "graphSize" {
                    tempPS.graphSize = v.value2 
                    tempPS.directory = fmt.Sprintf("%s%s=%d%s", tempPS.directory, v.attributeName, v.value2, "_")
                }
                if v.attributeName == "cuttingType" {
                    tempPS.cuttingType = v.value2 
                    tempPS.directory = fmt.Sprintf("%s%s=%d%s", tempPS.directory, v.attributeName, v.value2, "_")
                }
                if v.attributeName == "selectionIntensity" {
                    tempPS.selectionIntensity = v.value1 
                    tempPS.directory = fmt.Sprintf("%s%s=%f%s", tempPS.directory, v.attributeName, v.value1, "_")
                }
                if v.attributeName == "benefit" {
                    tempPS.benefit = v.value2 
                    tempPS.directory = fmt.Sprintf("%s%s=%d%s", tempPS.directory, v.attributeName, v.value2, "_")
                }
                if v.attributeName == "cost" {
                    tempPS.cost = v.value2 
                    tempPS.directory = fmt.Sprintf("%s%s=%d%s", tempPS.directory, v.attributeName, v.value2, "_")
                }
                if v.attributeName == "edgeProbability" {
                    tempPS.edgeProbability = v.value1 
                    tempPS.directory = fmt.Sprintf("%s%s=%f%s", tempPS.directory,v.attributeName, v.value1, "_")
                }
            }
            tempPS.directory = fmt.Sprintf("%s%s", "output/", tempPS.directory)

            //block until we have resources
            for threadCount > maxThreads {
            }
            threadCount++
            go func() {
                simulateGraphs(tempPS.graphSize, tempPS.selectionIntensity, tempPS.benefit, tempPS.cost, tempPS.numReps, tempPS.directory, tempPS.edgeProbability, tempPS.fullOutput, tempPS.cuttingType)
                fmt.Println(combinationNumber, "/", len(combinations), tempPS.directory)
                threadCount--
           }()
        }

        //block until all threads are done
        for threadCount > 0 {
        }

        fmt.Println("end? y/n")
        scanner := bufio.NewScanner(os.Stdin)
        scanner.Scan()
        if testInput(scanner.Text(), yninput) == "y" {
            break
        }
        
    }
}

