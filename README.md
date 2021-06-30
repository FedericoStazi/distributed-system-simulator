# distributed-system-simulator

A *dsitributed system* is a system in which many computers communicate to execute a distributed algorithm.
Here computers are called **nodes**, and communicate through **messages**.

`distributed-system-simulator` can be used to simulate a (relatively) abstract model of such systems.
In this model, there is a set of messages (`M1` and `M2` in this example), and each node's code looks like this:
```
on start:
  ...
 
on message M1:
  ...

on message M2:
  ...
```
To simulate such model, see the **Network**, **Messages** and **Nodes** sections.



In order for this model to be more realistic some additional constraints and characteristics can be defined.

The network connecting the nodes in the system cah have the following behaviours:
- Reliable and instantaneous: Messages are received immediately.
- Reliable: Messages are always received, but transmission is not instantaneous. Depending on how latencies are defined, this may mean that messages can be reordered.
- Fair-loss links: Messages could be lost or duplicated. The only constraint is that if a node keeps sending a message, this will eventually be received.
- Arbitrary links: Messages can be lost or duplicated indefinitely.

The nodes in the system cah have the following behaviours:
- No-crash and No-pause: Nodes never crash and never pause execution.
- No-crash: Nodes never crash, but may sometimes pause execution.
- Crash-stop: Nodes can pause and crash. If they crash, they will never resume execution.
- Crash-recovery: Nodes can pause and crash. If they crash, they may resume execution at some point in the future, but lose all the current work.

To add these (or other) behaviours in a model, se the **Behaviours** section.


## Network

This class simulates the network. Using `addNode`, `emplaceNode` and `emplaceMultipleNodes` nodes are added to the network. 
To add behaviour objects, methods `addNetworkBehaviour` and `addNodesBehaviour` can be used. If not used, default behaviours will be used.
After doing this, `start` is used to start the simulation, which involves node events and message passing according to the defined times.


## Messages

Any user defined class can be a message, as long as it is a subclass of `dssim::Message`.
`dssim::MessageID` provides an interface to identify messages (which may be required for some algorithms). A possible implementation of this is `dssim::MessageUID`.


## Nodes

A node is a subclass of one or more `dssim::Accepts<T,x>` classes. It should be read as: *The node accepts messages of type T, and the code runs in x ms*.

Defining the execution time of the code for each message allows for realistic simulation 
(e.g. simulating a node that provides some expensive computation and testing how many requests can be handled).
If a node receives a message it does not accept, this will be automatically droppped.

Such node will need to implement the following methods:
- `void onStart()`. This is called when the simulation starts. It has no specified running time, since it should only simulate what happens when a computer is turned on.
(e.g. sending a message to all other nodes to let them know of its existence).
- `void onMessage(T message)` (for all accepted messages `T`). This is called when a message of type `T` is received. 
The message is passed as argument because the message may be carrying data required by the algorithm.

The following methods can be used for nodes communications:
- `getID()`. Get the node's unique id. It is the node's responsibility to let other nodes know about its id if required in the algorithm.
- `sendMessage(T message, int receiver)`. Send a message to a specific node (using its id).
- `void broadcastMessage(T message)` Send a message to all nodes.
- `sendSelfMessage(T message)`. Send a message locally. This message is received reliably and instantly. 
The only difference with a regular function call is that doing this ensures that the times of the calling and called functions are counted correctly.
- `startTimer(T message, double duration)`. Start a timer that after `duration` ms sends a message to the node itself. 
`startTimer(message, 0)` is the same as `sendSelfMessage(message)`.

## Behaviours
