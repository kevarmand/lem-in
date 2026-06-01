# lem-in

`lem-in` is an algorithmic project from the 42 curriculum.

The goal is to move a given number of ants from a start room to an end room through an anthill represented as a graph.

At each turn, an ant may move from one room to another through a link. The main constraint is that every intermediate room can contain at most one ant at a time. The start and end rooms are the only exceptions.

Therefore, the problem is not only to find a path from `start` to `end`. The real goal is to find a movement strategy that minimizes the total number of turns required to move all ants to the end.

---

# Mandatory Part

The mandatory part of the project is divided into two main components:

1. **Parsing** the input and building a valid graph representation.
2. **Solving** the anthill by selecting an efficient set of compatible paths and dispatching ants through them.

The most important part of the project is the algorithmic one: the program must not simply find a path, but must find a set of paths that gives the best theoretical number of turns among the solutions considered by the solver.

---

# Input Parsing

The parser reads the anthill from standard input.

A valid input contains:

* the number of ants;
* a list of rooms;
* the special commands `##start` and `##end`;
* a list of links between rooms;
* optional comments.

The parser validates the input while building the internal representation of the graph.

It checks that:

* the number of ants is valid;
* there is exactly one start room;
* there is exactly one end room;
* room names are valid;
* room names are unique;
* room coordinates are valid according to the project rules;
* each link connects two existing rooms;
* duplicated or invalid links are rejected;
* the final graph can be used by the solver.

Invalid input stops the program with `ERROR`.

---

# Internal Representation

The parser converts the textual input into a graph-oriented structure used by the solver.

## Farm

The farm is the central structure of the program.

It stores:

* the total number of ants;
* all rooms;
* the start room;
* the end room;
* the links between rooms;
* the data required by the pathfinding and solving steps.

The farm is the bridge between the parser and the algorithm.

## Rooms

Each room stores:

* its name;
* its coordinates;
* its internal numeric identifier;
* its adjacency list;
* temporary fields used by graph algorithms.

The room name is kept for the final output, while the solver mostly works with internal numeric identifiers for efficiency and clarity.

## Links

A link represents an undirected connection between two rooms.

In the input, a link `A-B` means that an ant may move from $A$ to `B` or from `B` to $A$.

For the solving step, the graph can be transformed into a more convenient representation, especially when room capacity has to be enforced.

---

# Problem Model

Let the anthill be modeled as an undirected graph:

$$
G = (V, E)
$$

where:

* `V` is the set of rooms;
* `E` is the set of links;
* `s` is the start room;
* `t` is the end room.

A path from `s` to `t` is valid if it is a simple path in the graph.

The length of a path $P_i$ is written:

$$
\ell_i
$$

where $\ell_i$ is the number of moves required for one ant to travel from `s` to `t` through that path.

Two paths are compatible if they do not share any intermediate room:

$$
P_i \cap P_j \subseteq {s, t}
$$

This condition is essential because intermediate rooms have capacity `1`.

---

# Why Compatible Paths Matter

## Lemma 1 — Parallel paths must be room-disjoint

Let $P_i$ and $P_j$ be two paths from `s` to `t`.

If both paths share an intermediate room $v$, then they cannot behave as two independent parallel lanes.

### Proof

Assume:

$$
v \in P_i \cap P_j
$$

with:

$$
v \neq s \quad \text{and} \quad v \neq t
$$

The room $v$ is an intermediate room.

By the project rules, an intermediate room can contain at most one ant at a time.

Therefore, every ant using $P_i$ and every ant using $P_j$ must compete for the same capacity-one resource $v$.

So the two paths cannot provide two independent units of throughput.

Thus, to be used as parallel lanes, paths must not share intermediate rooms:

$$
P_i \cap P_j \subseteq {s, t}
$$

This proves the lemma.

---

# Capacity of a Fixed Set of Paths

Let:

$$
\mathcal{P} = {P_1, P_2, ..., P_k}
$$

be a set of compatible paths with lengths:

$$
\ell_1, \ell_2, ..., \ell_k
$$

Let $x_i$ be the number of ants assigned to path $P_i$.

The total number of ants is:

$$
\sum_{i=1}^{k} x_i = A
$$

where $A$ is the total number of ants.

## Lemma 2 — Completion time of a fixed dispatch

For a fixed compatible path set and a fixed ant distribution `x`, the minimum number of turns is:

$$
T(\mathcal{P}, x) = \max_{1 \leq i \leq k}(\ell_i + x_i - 1)
$$

### Proof

Consider one path $P_i$ of length $\ell_i$.

The first ant assigned to this path cannot arrive before turn:

$$
\ell_i
$$

because it must perform exactly $\ell_i$ moves.

If $x_i$ ants use the same path, consecutive ants must be separated by at least one turn on that path.

The earliest possible arrival turns on $P_i$ are therefore:

$$
\ell_i,\ \ell_i + 1,\ \ell_i + 2,\ ...,\ \ell_i + x_i - 1
$$

So the last ant assigned to $P_i$ cannot arrive before:

$$
\ell_i + x_i - 1
$$

This gives the lower bound:

$$
T \geq \max_{1 \leq i \leq k}(\ell_i + x_i - 1)
$$

This bound is reachable because the paths are compatible. Since they do not share intermediate rooms, one ant can be sent on each path every turn without collisions between different paths.

Therefore, the lower bound is exact:

$$
T(\mathcal{P}, x) = \max_{1 \leq i \leq k}(\ell_i + x_i - 1)
$$

This proves the lemma.

---

# Theoretical Score of a Path Set

## Theorem 1 — Minimum number of turns for a fixed path set

For a fixed compatible path set:

$$
\mathcal{P} = {P_1, P_2, ..., P_k}
$$

with path lengths:

$$
\ell_1, \ell_2, ..., \ell_k
$$

The minimum number of turns required to move $A$ ants is the smallest integer $T$ such that:

$$
\sum_{i=1}^{k} \max(0, T - \ell_i + 1) \geq A
$$

### Proof

A path $P_i$ can deliver at most $x_i$ ants by turn $T$ if:

$$
\ell_i + x_i - 1 \leq T
$$

which is equivalent to:

$$
x_i \leq T - \ell_i + 1
$$

If:

$$
T < \ell_i
$$

then no ant can arrive through $P_i$ by turn $T$.

Therefore, the capacity of path $P_i$ by turn $T$ is:

$$
\max(0, T - \ell_i + 1)
$$

The total capacity of the path set by turn $T$ is:

$$
C_{\mathcal{P}}(T) = \sum_{i=1}^{k} \max(0, T - \ell_i + 1)
$$

If:

$$
C_{\mathcal{P}}(T) < A
$$

then the path set cannot deliver all ants by turn $T$.

If:

$$
C_{\mathcal{P}}(T) \geq A
$$

then there exists a distribution of ants over the paths such that all ants arrive by turn $T$.

Thus, the optimal score of the path set is:

$$
S(\mathcal{P}) = \min \lbrace T \in \mathbb{N} \mid \sum_{i=1}^{k} \max(0, T - \ell_i + 1) \geq A \rbrace
$$

This proves the theorem.

---

# Consequence: Length Sum Matters

For a fixed number of paths $k$, if all paths are active at the optimal turn $T$, then:

$$
C_{\mathcal{P}}(T) = \sum_{i=1}^{k}(T - \ell_i + 1)
$$

So:

$$
C_{\mathcal{P}}(T) = k(T + 1) - \sum_{i=1}^{k}\ell_i
$$

At a fixed number of paths, the capacity depends on the sum of the path lengths.

This explains why two path sets with the same number of paths and the same total length have the same theoretical capacity, as long as all paths are active.

For example:

$$
(4, 6, 12)
$$

and:

$$
(7, 7, 8)
$$

both have three paths and total length:

$$
22
$$

In the active-path regime, they give the same theoretical capacity.

Therefore, the solver does not only maximize the number of paths. It evaluates the real number of turns obtained by each compatible path set.

---

# Rejected Approaches

Several classical graph algorithms were considered before choosing the final approach.

## Direct BFS

A direct BFS finds the shortest path from `start` to `end`.

This is not enough because the shortest path can use rooms that block several other paths. A locally optimal shortest path can therefore destroy a globally better set of compatible paths.

BFS is useful as a building block, but it cannot solve the full optimization problem alone.

## Dijkstra

Dijkstra solves a shortest path problem in a weighted graph.

`lem-in` is not a single-path problem. The objective is to optimize a set of compatible paths under room-capacity constraints.

Dijkstra optimizes one trajectory, while this project requires optimizing the global dispatch of all ants.

## Edmonds-Karp

Edmonds-Karp computes a maximum flow.

With the correct graph transformation, it can find the maximum number of compatible paths. However, maximizing the number of paths is not always equivalent to minimizing the number of turns.

A larger flow with very long paths can be worse than a smaller flow with shorter paths, especially when the number of ants is small.

Edmonds-Karp is therefore relevant for reasoning about capacity, but insufficient as the only optimization criterion.

## Dinic

Dinic also computes a maximum flow, usually faster than Edmonds-Karp.

It improves the performance of the flow computation, but it still optimizes the same objective: the value of the flow.

The project requires optimizing the final number of turns, not only the maximum flow value.

For this reason, Dinic was not the core of the final scoring strategy.

---

# Solver Strategy

The solver searches for compatible sets of paths and evaluates them using their theoretical score.

For each candidate path set:

1. the paths are checked for compatibility;
2. their lengths are collected;
3. the theoretical minimum number of turns is computed;
4. the candidate is compared with the current best solution.

The selected solution is the path set with the best score among the generated candidates.

The score used by the solver is:

$$
S(\mathcal{P}) = \min \lbrace T \in \mathbb{N} \mid \sum_{P_i \in \mathcal{P}} \max(0, T - \ell_i + 1) \geq A \rbrace
$$

This means the solver compares solutions using the actual number of turns they can achieve, rather than using only shortest-path length or maximum flow size.

---

# Fixed Path Simulation

Once the best path set is selected, each ant is assigned to one of these paths.

The path of an ant remains constant during the simulation.

This is not a limitation of the output phase. It follows from the model used by the solver: the selected path set already defines the available independent lanes, and the dispatch step fills those lanes according to their capacity.

Changing the path family during the simulation would only be useful if it created additional independent capacity.

However, if two path families are incompatible, their union does not form a larger compatible path set. They share intermediate rooms, and these rooms remain capacity-one bottlenecks.

Therefore, an apparent hybrid strategy does not create a capacity equal to the sum of both families. It only changes the order in which ants use already limited rooms.

The solver instead evaluates complete compatible path sets and keeps the one with the best theoretical score.

---

# Output

After selecting the best path set and assigning ants to paths, the program prints the movement sequence.

Each movement respects the project rules:

* an ant moves at most once per turn;
* an ant moves only through existing links;
* an intermediate room never contains more than one ant;
* the start and end rooms may contain several ants;
* every ant eventually reaches the end room.

The output format follows the subject requirement:

$$
Lx-room
$$

where `x` is the ant number and `room` is the destination room for this move.

---

# Summary

The project is not solved by a simple shortest path algorithm.

It is also not fully solved by maximizing flow alone.

The real objective is to minimize the number of turns required to send all ants from `start` to `end` under room-capacity constraints.

For a fixed compatible set of paths, the exact theoretical score is:

$$
S(\mathcal{P}) = \min \lbrace T \in \mathbb{N} \mid \sum_{P_i \in \mathcal{P}} \max(0, T - \ell_i + 1) \geq A \rbrace
$$

The solver uses this criterion to compare path sets and select the best candidate generated by the algorithm.

This makes the algorithm directly aligned with the real scoring objective of `lem-in`: minimizing the total number of turns, not only finding a path or maximizing raw flow.
