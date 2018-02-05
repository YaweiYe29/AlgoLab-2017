# AlgoLab-2017
My solutions on Algorithms Lab course at ETH Zurich. Not all ideas come from my head. Some come from repositories of [NorfairKing](https://github.com/NorfairKing/eth-algolab-2015) and [Timethy](https://github.com/timethy/algolab). Thank you guys!

## Prerequisites
  * C++/STL
  * Greedy, Divide & Conquer
  * Binary Search, BFS, DFS
  * Dijkstra, MST
  * Graph teory
  * Complexity teory
  * Graph flows
  
## What will you learn
  * Apply algorithms properly
  * CGAL and BOOST libraries
  * Geometric algorithms
  * Linear and Quadratic programming
  * Max flow min cost, min cuts
  * Max bipartite matching, minimum independent set
  
## Course Overview
  * One of the 2 courses that every ETH CS master student have to take
  * 2 hours lecture + 2 hours problem of the week
  * Be prepared to spend up to 20 hours solving various algo tasks
  * Very similar to informatics olympiads
  * One of the most difficult courses at ETH

# SPOILERS - Solutions
| Week   | Problem   | Points (out of 100)   | Solution   |
| ------------- |:-------------:| -----:|:-------------------:|
| 1   | Build the sum    | 100   | primitive   |
| 1   | Even pairs    | 100   | Precomputed sum + combinatorics   |
| 1   | Even matrices     | 100   | The same as Even pairs but in matrix   |
| 1   | Dominoes   | 100   | For loop + memo var   |
| 1   | False coin   | 100   |Items tagging   |
| PoW1  | Deck of cards | 100   | Sliding window O(n) (I got 100 even with double for loop   |
| 2   | Boats   | 100   | Sort + kind of Optimal scheduling   |
| 2   | Books   | 100   | Sort + stl tricks   |
| 2   | Evolution   | 100   | Tree + DPS + Skip list   |
| 2   | Search snippets   | 100   | Sort + map + interesting array iteration   |
| PoW2   | Octopussy   | 100   | Recursion + tagging   |
| 3   | Hit   |   100    | Intersection   |
| 3   | Antenna   | 100   | Min circle   |
| 3   | AlmostAntenna | 100   | Support points + min circle   |
| 3   | First Hit   | 99   |   Intersection   |
| 3   | Hiking maps   | 100   | Inside triangle test + duplicates removal   |
| Pow3   | Return of the Jedi | 0   | ???   |
| 4   | Ant challange   | 100   | Multiple graphs + MST + Dijkstra   |
| 4   | Buddy selection   | 100    | Set intersection + Max cardinality matching   |
| 4   | Graphs   | 100   | MST + Dijkstra   
| 4   | Important bridges | 100 | Biconnected components   |
| 4   | Shortest paths   | 100   | Dijkstra   |
| PoW4   | Tracking   | 100   | K-level graph + Dijkstra   |
| 5   | Burning coins   | 100   | DP + memo(2D array -> (left, right) -> best game   |
| 5   | Light pattern   | 100   | O(n) + right to left switching   |
| 5   | Light at the museum   | 100   |Bruteforce + split and list O(2^(n/2) * n)   |
| 5   | Notebook   | 100   | String hashing unordered_map   |
| 5   | The great game   | 100   | Similar to bur. coins but much harder. DP + memo (one for even one for odd moves)   |
| PoW5   | Motorcycles   | 100   | 128b Integer + slope comparing + Sort   |
| 6   | Coin tossing   | 100   | Easy max flow   |
| 6   | Kingdom defence   | 100   | Min flow per edge   |
| 6   | Shopping   | 100   | Doubled graph + max flow   |
| 6   | Tetris   | 100   | Max flow   |
| PoW6   | Poker chips   | 100   | Huge DP + 5D memo   |
| 7   | Max   | 100   | QP + negate obj. funtion + negate obj. value   |
| 7   | Diet   | 100   | LP   |
| 7   | Inball   | 100   | LP   |
| 7   | Portfolios   | 100   | QP as in lecture slides   |
| PoW7   | Marathon   | 100   | All shortest paths, Dijkstra from both sides + iterate over edges   |
| 8   | Bistro   | 100   | DT + nearest_vertex   |
| 8   | Graypes   | 100   | Edges in DT form Near. neigh. graph -> iterate over them   |
| 8   | Germs   | 100   | Edges in DT form Near. neigh. graph -> vertex circulator   |
| 8   | H1N1   | 60   | Dijkstra + DT + BFS + precomputation   |
| PoW8   | Lestrade   | 100   | LP + DT   |
| 9   | Algocoon   | 100   | Multiple Min cuts   |
| 9   | Canteen   | 100   | Non negative weights + fridges as edges to later days   |
| 9   | Real estate   | 100   | Const for non neg. weights, Max flow min cost   |
| 9   | Satellites   | 100   | Max bipartite matching   |
| PoW9   | Surveillance Photographs   | 100   | Max flow. Similar to Shopping...   |
| 10   | On her majesty | 100   | Dijkstra for every agent + bipartite matching + double for 2 shelters   |
| 10   | Bonus Level   | 100   | 2 traverses in one + 2D memo (diagonal + x)   |
| 10   | Light the stage   | 100   | DT + remember who kill every person   |
| 10   | Return of the jedi   | 100   | MST + DFS + max edge from to   |
| PoW10   | Sith   | 100   | Binary search for max component + DT   |
| 11   | Carsharing   | 100   | Max flow min cost + tricky bias and edge cost   |
| 11   | Defensive line   | 100   | Table DP (2D table for players hit and attackers) + sliding window   |
| 11   | Punch   | 100   | Table DP (2D table for max drinks and min price)   |
| 11   | Strikesback   | 100   | LP + DT   |
| PoW11   | High school teams   | 100   | Bottom up DP with split and list   |
| 12   | Beach bars   | 100   | Greedy + Sort   |
| 12   | Casino royal   | 100   | The same as Carsharing   |
| 12   | Golden eye   | 100   | DT + Union find   |
| 12   | New tiles   | 0   | Bit playing DP   |
| 12   | Placing knights   | 100   | Max cardinality matching   |
| 12   | Radiation   | 0   | LP   |
| PoW12   | WorldCup   | 100   | DT + LP + realize that we never cross same circle twice i.e. we are inside it   |
| 13   | Bobs Burden   | 100   | 3x Dijkstra + out in edges + iterate over artificial edges   |
| 13   | Clues   | 100   | DFS + 2 coloring + DT + connected compomenents   |
| 13   | Corbusier | 100   | Table DP for 100p + split and list for 60p   |
| 13   | Goldfinger | 100   | DT + LP + similar to strikesback   |
| 13   | Phantom Menace | 100   | Min vertex cut   |
| 13   | Planks   | 100   | Split and List DP + sets as bits   |
| PoW13   | Fleetrace   | 100   | Max weighted bipartite matching   |
| 14   | Cantonal Curier   | 100   | Max flow no idea why it works   |
