# Optativa 02, Prueba técnica Hadean

## Fecha Entrega, Conv1 = 09/02/20

For a data processing task, we have an input file where each byte represents a symbol in an input sequence. We wish to store this sequence in RAM and be able to access a symbol at a given index in constant time . However, the large length of the sequence is problematic due to memory constraints. The symbols in the sequence generally occur in a non-repeating order, and the probability distribution of the symbols is close to uniform. In practice, the number of unique symbols in the sequence is small (often ≤ 16) but may take arbitrary values. For example, when rendered using ASCII, an input file might contain:

RYKARRKAAGCWATYWKMTCTWRAGSRMGCTKGTKCACMYACYTTMG

In the language of your choice, in an imperative style, construct the following:
- An application which takes an input file and produces an output file which contains a compact representation of the input file.
- An application which takes the compact representation and regenerates the original file.

Your compact representation should be designed so that it would be possible to implement functionality that loads the compact file into memory (without occupying significant extra space) and access symbols at arbitrary indices in constant time. For this exercise, we do not require that you implement this functionality, just the reconstruction of the original file.

We do not require that your compact representation is the most efficient possible, but it should at least offer significant space savings for alphabet sizes ≤ 16 and have the property that smaller alphabet sizes compress more efficiently.

The input file should be treated as binary rather than text so that the reconstructed file is bit-identical to the original input file. The format you choose for your output file can be arbitrary.

Please include a README file explaining your design choices and any features and / or limitations of your implementation that you feel are important.

We will evaluate the exercise on the following criteria:
- Code clarity and correctness.
- Error detection and handling / reporting (this need not be more complex than printing a comprehensible error message then exiting, but it should not be easily possible to crash your code).
- Appropriate use of commenting.
- Documentation on how to build and run your solution.
- Use of appropriate build tools.
- Edge case handling.
- Presence of automatable testing.

Performance optimisation is not a primary focus of this exercise and should not be undertaken in a way that comprimes the other listed criteria. However, implementations that have pathological performance issues are undesirable. You may assume that the machine on which your application executes has sufficient memory to hold the entire input sequence (it is the later computation on input sequence that necessitates its efficient storage in memory due to the presence of other memory-hungry data structures).