# sim
Competitive games on graphs simulator with cutting ties.

The bulk of the work is done in functions.c. The .sh and golang scripts enable you to create set of attributes to simulate and run efficiently using golang's goroutines. run from simulator.go for normal functionality or just use main.c as an entry point supplying the relevant attributes however you so choose.

Evolutionary graph theory is the study of games played on graph networks. The be-
haviour of these games can vary wildly depending on the attributes given to the graphs
and the rules set for the games played on them. In the last few decades more focus
has been paid to networks which are dynamic and have games played on them which
contain rules which reference the neighbourhood of individual nodes on the network.
Particular attention has been paid to networks in which rules of cooperative and non-
cooperative play have been set. The death-birth mechanism in such games seeks to
emulate the ebb and flow of cooperation in real-world networks. One area which has
come into recent focus is the effect of tie-cutting. Destroying links between nodes on
the network, based on criteria which includes reputation of neighbouring nodes as a
function of the value acquired playing the game, random selection and others. A study
of the effect these different strategies have on the outcome of these games and the
effect different variables have on these strategies is reported here.
A comparison has been made between the data gathered in this study and others in
the same area, showing more evidence that cooperators thrive in environments which
have a low selection intensity and, low connectivity and small populations. This project
further affirms the notion that cooperators thrive when n as a multiplier of payoff (the
value gained by a node for playing the game) is n < 0.001. Comparisons have been
made to other studies with similar results using humans and animals. Results per-
taining to the tie-cutting strategy are inconclusive and not obvious using the statistical
methods used in this project.
The project does affirm the aspects of cooperative games described in previous
papers. No conclusions about relationships between tie-cutting strategy and other at-
tributes of graphs were able to be made. A comparison to other papers in this area
show that other statistical methods may be more fruitful.
