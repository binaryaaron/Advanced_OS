The L4 family of microkernels is well-studied and has been through many
iterations with various tweaks and design changes over the years. The authors
detail the many kinds of changes made and original design and implementation
decisions kept from the original L4. L4 was developed from L3, finding success
in the systems/OS research spectrum (and leading to a bit of commercial
success) due to an order-of-magnitude improvement in IPC speed over previous
generations of microkernels, which had been a key performance sore spot in
microkernels. (L4 was able to perform IPC in roughly 5 microseconds when
previous microkernels were hovering around 100 microsecond IPC times).
Liedtke’s success started a cascade of L4 implementations and reimaginations,
including architecture-specific versions (L4/MIPS), embedded systems
distributions, and a formally-verified version designed for security (seL4).
The paper mostly speaks to the seL4 microkernel, as they claim it is the most
evolved version in the L4 family in terms of design, though some design choices
seem to have been made for convenience of verification. The vast majority of
the paper focuses on the design and implementation of the L4 family, the
benefits and contributions of which will be discussed below. The paper does not
give much quantitative data about the costs and benefits of these design
choices and does not often compare them to other kernel designs (e.g., Linux). 

The overall design of most microkernels includes a level of minimality - all
the building blocks of a monolithic OS would be put into user space. Only tasks
that would prevent implementation of required functionality would be done in
the microkernel. This has continued to drive modern designs of L4, with low
codebase sizes being cited some proof of minimality. This clearly helped make
seL4 verifiable, a theme that does continue to show up throughout the paper.
The minimality aspect of all microkernels has had a lasting effect in both
research and industry.

The IPC system perhaps saw the most evolution over time. Synchronous IPC was
augmented with asynchronous notifications (and later endpoints), allowing
easier control over threads in multicore environments despite breaking
minimality. The structure of IPC messages were changed from an in-register
message to a hybrid model of virtualized registered and per-thread space
abstracted away from a user. Long IPC was abandoned as it was infrequently used
and could create page faults, which in turn made it difficult to verify what
was going on in seL4. IPC timeouts were removed as a potential denial of
service attack point defense. The work in optimizing IPC clearly had great
effect on microkernel design and lead to improvements in virtualization (e.g.,
better message passing for hypervisors). 

In the L3 tradition, nearly all device drivers are user-level processes -
again, verification is cited as a primary reason for doing this. Device drivers
are often buggy and are a leading cause of system instability, so this seems
like a reasonable approach and is likely the biggest paradigm shift from the
monolithic kernel method for handling drivers and builds off of the work of
other similar microkernels. 

Recursive page mapping was a major feature of the L4 family and is an elegant
construct for managing memory. The model was expensive and eventually this was
replaced in seL4 with a more standard model. Other L4 family designs use
different models of memory management, and perhaps there is more variance here
than in any other area of the L4 family. Kernel memory was a difficult task to
implement, with the original fixed-size kernel heap being replaced with various
forms of management, mostly to prevent security issues. seL4 completely removes
the kernel heap from the kernel and give user space a means to see what is
kernel memory. Kernel objects are explicit and under strict capability based
access control and tracked, and seL4’s model results in a (yet-again)
verifiable approach to handling kernel memory.

Multiplexing of the CPU is handled with a fixed-policy scheduler. In one of the
best quotes i’ve seen in a research paper, “...hard-priority round-robin, is
still alive, despite being a gross heresy against the core microkernel religion
of policy freedom”, brings the problem of substantial overheads in putting
scheduling in user space. A policy free approach to scheduling would fit well
within the realms of verification and purity of design. Multiprocessor systems
prove difficult for verification and seL4’s hammer-sized solution of putting a
lock on the kernel is claimed to be not terribly impactful on performance, but
they could run into contention issues when dealing with various tasks in
certain use cases. They leave the multicore case as ‘unresolved’, though
breakthroughs in this area would open up greater processing ability for
microkernel systems. 

Replacing the original per-thread kernel stack by event-based kernel proved
useful for various reasons, including verification, improved performance on
multiprocessing tasks, and lowered overall memory usage. This also has the
benefit of simplifying the design. Lazy scheduling, a method to reduce thread
queue manipulations during IPC between threads in the queue, was replaced due
to poor performance with Benno scheduling. L4 kernels traditionally tried to
not run the scheduler during IPC but again, seL4’s needs for verification lead
to a modification in this by keeping priorities straight with direct-process
switching and using the scheduler when they don’t conform with priorities. seL4
also helped keep non-preemption in the kernel by need of verification
(concurrency in the kernel is problematic for verification). Portability was
implemented by keeping much of the codebase architecture-agnostic, but seL4 and
NOVA both still have 50% of their codebase architecture specific. Verification
of seL4 was a motivator for moving from a non-standard calling convention to
using C with a splash of assembler for IPC optimization, though this still
proved problematic for verification and eventually assembler was abandoned for
C.

The paper, while providing a wealth of information about the L4 family, focuses
entirely too much on seL4 and leaves a sense of the authors basing all design
decisions around verification. There is nothing inherently wrong with this, as
the verification is seminal and fascinating, but more could have been said
about the non seL4 variants. In addition, more quantitative data, particularly
comparing performance evolution or tables summarizing certain changes would be
helpful for this reader. 






The L3/L4 variants had a wide range of structures that evolved over the years;
however, they have used basically a single scheduling strategy over the years
despite many efforts to do otherwise. Compare and contrast their approach
versus multi-level feedback scheduling in Linux and scheduling activations. In
particular, how would a user implement approaches like these on an OS like L4,
and what would be the costs and benefits of doing so as opposed to putting more
complex scheduling support in the L4 kernel?

c&c hard-priority rr with MLFS and scheduling activations. 




Background: The multilevel feedback queue (MLFQ) seeks to minimize turnaround
time and response time.  Each level in the MLFQ represents a priority value.
Priority values are learned by job behavior: short jobs that relinquish control
(e.g., keyboard interrupts) stay with high priority and long-running jobs get
lower priority. Two basic rules for MLFQs are: 

If Priority(A) > Priority(B), A runs (B doesn’t) If Priority(A) = Priority(B),
A & B run in round robin

Scheduler Activations (SA) are/is a threading mechanism that provides
kernel-level thread functionality with user-level thread flexibility and
performance. SA uses an N:M strategy that maps some N application threads to M
kernel entities. SAs are not in as common of use as MLFQs. 

L4 kernels, to the dismay of the designers, mostly have a hard priority round
robin scheduler with various minor optimizations, e.g., lazy scheduling. At the
very least, this is simple, easy to implement and understand but lacks the
performance that MLFQs tend to bring.

Most of the issues with process scheduling in L4 involve incurred costs from
communication and context switching. Pushing the scheduling mechanism to user
space comes at a serious price, mostly one of heavy costs of IPC when processes
request access to a service and also creates many context switches. Synchronous
message passing also introduces dependencies between components, and when
threads have hard priorities, this results in a system susceptible to priority
inversion. Implementing an MLFQ in the microkernel space is orthogonal to
design decisions of policy freedom in the kernel space. 










L4 retains user device drivers across all of its versions, giving it much (if
not more?) of the functionality provided by Nooks.  Compare and contrast the
fault models and support provided by the system kernel for recovery. 

Similarly, if the same fault were to occur in a network device driver in L4 and
in Linux/Nooks, what portions of recovery would be automatically handled in
each system, and what would require the user/device driver author/server author
to add extra support for recovery in each system?


---

L4 can be thought of as a privilege level change of drivers, which prevents
device drivers from executing privileged instructions and/or emulate privileged
instructions. L4’s model does not require driver rewrites, supports easy
recovery from driver faults, has good performance for large data transfers,
isolates memory corruption, and prevents most deadlocks, while not having high
performance for small data transfers. Driver support is implemented as each
driver having it’s own memory space with virtualized memory (dataspace
managers), where driver interrupts are handled via kernel objects and IPC. I/O
ports and memory have virtualized mappings and all IO is funneled through a
common tunnel to the kernel.                      
                        
                                
Nooks was designed from the ground up for protection from device faults, and as
such uses a hybrid of several methods, including software fault isolation and
kernel wrapping. The Nooks architecture supports two levels of recovery: full
restart, which unloads and restarts drivers, and rollback, which uses
recoverable virtual memory to maintain a shadow copy of driver state, allowing
it to be recovered after a fault. Nooks forwards interrupts from devices to
device drivers and can add safety by emulating access to memory-mapped device
registers. OS system kernel calls to device drivers are wrapped by nooks to
verify data that is passed through the kernel. All drivers execute in kernel
space but with various protections.

If a networking device generated a fault, Nooks would try to recover first by
using the shadow copy of the driver’s state and then restart if recovery
failed. By default, L4 family microkernels uses a ‘reincarnation’ server to
restart a crashed component, and can use the datastore area to restore the
device’s state, though ethernet devices are stateless and do not need to do
this. This recovery procedure is covered in detail in Herder et al, 2007. 


Herder, J.N., Bos, H., Gras, B., Homburg, P., Tanenbaum, A.S.: Failure
Resilience for Device Drivers, Proc. Dependable Systems and Networks 2007, pp.
41-50, 2007 









Compare and contrast L4 IPC as originally designed and as it later evolved with
the Mach port abstraction. Which features did the L4 series of kernels end up
adopting and rejecting, and why?


Mach ports were the communication channel for Mach’s IPC. It is a protected
message queue for communications between tasks (a task is another abstraction
of memory that allows threads to run within its context). Ports are protected
by kernel capability measures (port rights) so that a task must have a right
before it can sent to a port. Groups of ports can be defined in sets
                        
Originally, L4’s IPC system supported synchronous IPC, rich messages,
register-based message passing, timeouts, thread-id as destinations, and a
communication control model called “clans and chiefs” for authentication and
authorization. 

L4 kernel’s evolution of IPC went through many phases. Synchronous IPC was
augmented with asynchronous notifications (and later endpoints), allowing
easier control over threads in multicore environments despite breaking
minimality. The structure of IPC messages were changed from an in-register
message to a hybrid model of virtualized registered and per-thread space
abstracted away from a user. Long IPC was abandoned as it was infrequently used
and could create page faults, which in turn made it difficult to verify what
was going on in seL4. IPC timeouts were removed as a potential denial of
service attack point defense. Clans and chiefs proved too costly, as each
chief-to-chief communication incurred extra messages and was too rigid of a
hierarchy. 

