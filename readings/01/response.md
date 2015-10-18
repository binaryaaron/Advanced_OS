mach - os

Transaction management systems for secondary stoarage and page replacement.

Explore goal, design, and implementation of mach and external mem management

Mach was potentially one of the first systems to make a move away monolithic
OS design and allowing the rise of microkernels via flexibility in process
communication. Perhaps Mach's most important abstraction was the architecture
of virtual memory as memory objects accessed via "messages" which grew forth
from the previous Accent kernel. This design allows for accessing the same
region in memory across various processes and threads, enabling straightforward
multiprocessing. Each memory object is merely an object with a minimal set of
operations (read, write, etc.) are defined and the Mach kernel manages the
cache of each object. It is interesting to see that some of these abstractions
around messaging were essentially brand-new at the time and just how rapidly
changing the field of computer science is - with the original Unix OS being
release only in 1973.
