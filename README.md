hs11-megalopolis
================

Repository of the HS2011 contest problem. Problem code: HS11MEGA

Problem statement:

As the outlook of the global economy is looking bleaker and bleaker, more and more countries are facing financial difficulties. One of the hardest hit by the crisis is Byteland. Essential cuts are needed in order to keep this sinking boat afloat. Infrastructure is one of the key areas where great savings can be made...

Byteland consists of a number of cities. Bytelanders travel between them using a network of one-way motorways. Every motorway connects two cities and has a precisely calculated annual maintenance cost. As a result of extensive urbanization, some of the cities form megalopoli. A megalopolis is a structure consisting of one or more cities, so that one can travel freely between any two cities belonging to it. The Minister of Infrastructure is planning to close some of the motorways in order to save money. Unsurprisingly, his goal is to save as much as possible.

That is why you, a leading Bytelandian programmer, were asked to write a program that will help him decide which motorways to abandon and which to keep, and were given the following guidelines:

Motorways connecting cities from the same megalopolis must not be closed,
Some motorways connecting cities from different megalopoli can be closed,
Every motorway connecting cities from different megalopoli, which you decide should be maintained, will be upgraded to allow two-way traffic.
Remember, the goal is to:

Minimize maintenance cost of Bytelandic road network and maximize savings.
Allow undisturbed travel between any two cities in Byteland, once the upgrade is completed.
Input

The first line of input consists of two numbers N (2 <= N <= 10^6) denoting the number of cities and M (N-1 <= M <= N*(N-1), M <= 10^6) the number of one-way motorways. The next M lines contain a description of Byteland's road network. The (i+1)-th line of input consists of 3 numbers A, B (1 <= A, B <= N, A != B) denoting starting and finishing cities and C (1 <= C <= 10^3) denoting annual service cost of a given motorway.

Output

The first and only line of output contains a number denoting maximal possible savings: a sum of annual maintenance costs of motorways that can be abandoned. If no savings can be made, you must output 0 (zero).

Example

Input:
7 11
1 2 100
3 4 256
2 6 78
4 7 76
7 6 91
4 5 123
1 3 33
2 7 67
5 6 131
2 1 997
5 3 70

Output:
298
Example explanation: There are 4 magalopoli: {1, 2}, {3, 4, 5}, {6}, {7}. Connections: (5 6), (7 6), (4 7) will be closed; connections: (1 3), (2, 6), (2, 7) will be upgraded.

Scoring

By solving this problem you score 10 points.
