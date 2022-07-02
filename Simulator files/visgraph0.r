library(igraph)
args = commandArgs(trailingOnly=TRUE)
if (length(args)==0) {
    stop("supply an argument (rscript)!", call.=FALSE)
}
homeDirList <- list.dirs(path = args[1], recursive = FALSE)
numGraphs <- sum(length(homeDirList), -1)

for (y in 0:numGraphs)
{ 
    print(y)
    stepDir = paste(args[1], "/graph_", y, "/step_0/adjStatus0.csv", sep = "")
    graphedges <- read.table(stepDir, sep=" ", header=FALSE)
    graphedgesmat <- as.matrix(graphedges)
    g <- graph_from_edgelist(graphedgesmat, directed=FALSE)
    l <- layout_with_lgl(g)

    dirList <- list.dirs(path = paste(args[1], "/graph_", y, "/", sep = ""), recursive = FALSE)
    numSteps <- sum(length(dirList), - 1)
    for (x in 0:numSteps)
    {
        stepDir = paste(args[1], "/graph_", y, "/step_", x, "/adjStatus", x, ".csv", sep = "")
        graphedges <- read.table(stepDir, sep=" ", header=FALSE)
        graphedgesmat <- as.matrix(graphedges)
        g <- graph_from_edgelist(graphedgesmat, directed=FALSE)
        jpeg(paste(args[1], "/graph_", y, "/", "step_", x, ".jpg", sep = ""))
        statusDir = paste(args[1], "/graph_", y, "/step_", x, "/coopStatus", x, ".csv", sep = "")
        coopStatus <- read.table(statusDir, sep=",", header= FALSE)
        for (x in 1:gorder(g))
        {
            if (coopStatus[x, "V2"] == 1)
            {
                V(g)[x]$color <- "blue"
            } 
            if (coopStatus[x, "V2"] == 2)
            {
                V(g)[x]$color <- "grey"
            } 
            if (coopStatus[x, "V2"] == 0)
            {
                V(g)[x]$color <- "red"
            } 
        }
        plot(g, layout=l, edge.color="#0000FF25", vertex.size=4,edge.width = 2,vertex.label.dist=1)
        dev.off()
    }
}
