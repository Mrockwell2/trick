# Trick CheckPointing Best Practices

**Contents**

* [Prerequisite Knowledge](#prerequisite-knowledge)<br>
* [Do's and Don'ts](#guidelines)<br>
* [Other Resources You Might Find Useful](#other-resources)<br>

***

Part of the process of designing a Trick simulation model is to ensure that it can be reliably checkpointed. Trick provides a lot of support for checkpointing, but there are things to know, and pitfalls to avoid. The purpose of this article is to provide knowledge, and guidelines that will make checkpointing easier.

<a id=prerequisite-knowledge></a>
## Prerequisite Knowledge

The following is a high-level overview of the Trick Memory Manager and checkpointing. Understanding these concepts are important, and will help you design your sim models to be reliably checkpointable.

<a id=how-memory-allocations-are-recorded></a>
### How Memory Allocations are Recorded

The Memory Manager is the component that **"knows"** about the memory objects (allocations) in your Trick simulation. For each of these objects the Memory Manager stores the following **"knowledge"** :

1. **Name** - optional, but **STRONGLY** recommended.
2. **Address** - the address of the memory allocation.
3. **Storage-class** - this is either
 	* **TRICK\_LOCAL** for memory that is allocated by the Memory Manager, or
	* **TRICK\_EXTERN** for memory that is allocated outside the Memory Manager that the Memory Manager is "told" about.
4. **Data-type** is
	* Primitive type specifier (```TRICK_DOUBLE```, ```TRICK_INT```, ... etc.) , or
	* Composite type specifier (```TRICK_STRUCTURED```). In this case the details of the type are specified by an ```ATTRIBUTES``` structure that is generated by Trick's Interface Code Generator (ICG).

<a id=trick-object-serialization></a>
### Trick Object Serialization

The Memory Manager can convert (ie., serialize) any of the objects that it **"knows"** about to a portable, human-readable text representation, to the extent that it knows about them (ICG can only gather data-type knowledge from header files that it has scanned.) The object can later be re-created from this representation. The represention consists of:

1. A **definition** of the allocation, and 
2. **value assignments** to each of the members of the allocations data type.

#### Example:

Suppose one were to perform the following allocation: 

```double *dbl_p = (double*)TMM_declare_var_s("double dbl_array[3]");```

The Memory Manager would represent its **definition** as follows in a checkpoint :

```
double dbl_array[3];
```

If one were then to assign values to the object, i.e. :

```
    dbl_p[0] = 1.1;
    dbl_p[1] = 2.2;
    dbl_p[2] = 3.3;
```

then the Memory Manager would represent its **variable assignment** as follows in a checkpoint :

```
dbl_array = 
    {1.1, 2.2, 3.3};
```
<a id=serialization-of-composite-objects></a>
#### Serialization of Composite Objects
For composite type objects (i.e., class & struct objects), the **variable assignment** can consist of many assignment statements. Trick check-pointing code recursively descends into the composite type-tree, writing an assignment statement for each of the primitive data-typed members (leaves).

<a id=serialization-of-pointers></a>
#### Serialization of Pointers
A pointer contains an address of another object. What's important is that it **refers** to the other object. We can't store the address of the object, because it will probably be different when the object is re-created at checkpoint reload. But, a **name** is also a reference. So we store pointers as names. Since objects have a name, and an address (once it's re-created) we can restore pointers by converting the name reference back to an address reference.

<a id=importance-of-naming-allocations></a>
#### Importance of Naming Allocations
If an object is named, then that name will be used in checkpointing, 1) to identify and 2) to refer (point) to the object. If the object is anonymous then a temporary name must be created for checkpointing. These temporary names are of the form ```trick_anon_local_<number>``` or ```trick_anon_extern_<number```. They can't be as descriptive as a name you might chose so, it's a good idea to name your allocations when possible. It will be a lot easier to find them in a checkpoint file.

<a id=simulation-checkpointing></a>
### Simulation Checkpointing

A **checkpoint** is a persistent representation of a simulation state. It's exactly like a "saved computer game" when it's time for dinner.

If the Trick Memory Manager **"knows"** about all of the allocations that comprise the state of a simulation, then it can checkpoint that simulation. The Trick Memory Manager checkpoints a simulation by :

1. Opening a checkpoint file.
1. Writing all the **definitions**, of all of the objects that it knows about, to the file.
2. Writing ```clear_all_vars();``` to the file. This is interpreted when the checkpoint is re-loaded, to initialize the re-created objects.
3. Writing all the **variable assignments** to the file. These will populate the values of the object when the checkpoint is re-loaded.
4. Closing the file.
  
There are certain things that simply cannot be checkpointed like file-pointers, and network connections. Perhaps there are other things as well. For these situations, Trick provides four special job classes: ```"checkpoint"```, ```"post_checkpoint"```, ```“preload_checkpoint”```, and ```“restart”``` (described below).

<a id=dumping-a-checkpoint></a>
### What Happens When You Dump a Checkpoint

A checkpoint of a simulation is usually initiated from the Input Processor. That is, via:

1. The input file, or
2. The variable server.
 
```trick.checkpoint( <time> )``` is called from Python. This Python function is bound to the corresponding C++ function. At a simulation frame boundary (so that data is time-homogeneous), three things happen:

1. The ```"checkpoint"``` jobs in the S_define file are executed. These job-classes allow you to prepare your sim to be checkpointed. Perhaps you want to transform simulation state data into a different form for checkpointing. This is up to you.

2. ```write_checkpoint( <filename> )``` is called. This writes the three sections of a checkpoint file as described above.
	
3. The ```"post_checkpoint"``` jobs are called. This too is an opportunity for your simulatiion to tidy up what you may have done in your ```"checkpoint"``` job. 

<a id=loading-a-checkpoint></a>
### What Happens When You Load a Checkpoint.
Trick.load_checkpoint() is called from Python.
At a simulation frame boundary, three things happen:

1. The ```“preload_checkpoint”``` jobs are called. These job-classes allow you to prepare your sim for a checkpoint-restore, in whatever way you see fit.
 
2. ```init_from_checkpoint( <filename> )``` is called, which:

	1. Calls ```reset_memory``` to delete all dynamically allocated objects.
	2. Calls ```read_checkpoint( <filename> )``` to read, parse, and restore the state described in the checkpoint file.
		* Read the **definitions** section of the checkpoint file, and allocate all of the objects described there in.
		* Clear all of the objects to 0, as appropriate to the data-type.
		* Read the **assignment statement section**, and assign values to the objects.

3. Run the ```“restart”``` jobs. These too are user-defined jobs that “tidy up” the simulation state. For example, this is where files might be re-opened, or socket connections are re-established. Again, what this does is up to the sim designer.
 
<a id=guidelines></a>
## Do's and Don'ts
* Plan for, and test that your models are checkpointable. Don't let this be an after-thought. Just like testing, if this is done from the beginning the pain suffering you’ll endure will be greatly reduced.

* Keep data types no more complicated than they need to be. Remember the KISS principle.

* If [I/O specification](https://nasa.github.io/trick/tutorial/ATutAnalyticSim#the-inputoutput-io-specification) of a data member is ```**``` then it will not be saved in a checkpoint. 

* Don't make anonymous allocations in the input file. Naming them will make it much easier to find them later.

* If you want your allocated objects to be checkpointed, allocate them with Memory Manager functions like: ```TMM_declare_var```, ```TMM_declare_var_1d```, and ```TMM_declare_var_s```.

* If you use the ```new``` or ```malloc``` to allocate memory, the trick memory manager will have no knowledge of the memory allocation and will not checkpoint it. In order to make trick aware, you need to use a trick memory manager function to allocate (as above) or use ```declare_extern_var```.

* In the case where you use ```declare_extern_var```, it's important to remember that that Trick will not delete nor restore objects that it did not allocate. It does not know how **extern** memory was allocated ( Note that ```new``` and ```malloc``` are not the only ways to allocate memory ). You are still responsible for de-allocation at the appropriate time.

<a id=other-resources></a>
## Other Resources You Might Find Useful
* [https://github.com/nasa/trick/tree/master/trick_source/sim_services/MemoryManager/test](https://github.com/nasa/trick/tree/master/trick_source/sim_services/MemoryManager/test)
* [https://github.com/nasa/trick/raw/master/share/doc/trick/advanced/Trick_Memory_Manager_Overview.ppt](https://github.com/nasa/trick/raw/master/share/doc/trick/advanced/Trick_Memory_Manager_Overview.ppt)
* [https://github.com/nasa/gunns/wiki/Trick-Checkpoint-Restart](https://github.com/nasa/gunns/wiki/Trick-Checkpoint-Restart)