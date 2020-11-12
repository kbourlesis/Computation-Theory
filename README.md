# Computation-Theory
This code takes an alphabet as input and creates a Deterministic Finite Automaton. Subsequently, it takes words as input from keyboard and prints messages if those words belong to its alphabet or not.

Example of alphabet file:
<pre>
3     // Number of states

0 1   // Symbols could be more or less than the given ones

0     // Start state. 0 represents q0

0 1   // Final states q0 and q1, could be more or less than the given ones

0 1 1 // Lines 10-15 are the transitions. For this line 0 is current state, 1 is incoming input and 1 is next state

0 0 0 // Line 11 example. 0 0 0 means, if automaton is in state 0 (q0) with 0 as input it will stay to state 0 (q0)

1 1 2 // Line 12 example. 1 1 2 means, if automaton is in state 1 (q1) with 1 as input it will move to state 2 (q2)
</pre>

1 0 0

2 1 2

2 0 2

Warning 1: Due to the different line endings between linux and windows, alphabet files should be saved based on linux line ending otherwise the program will not have the expected results.

Warning 2: The alphabet file should NOT contain any fewer or more spaces and line break characters than the existing ones because that way it will misread the data.
