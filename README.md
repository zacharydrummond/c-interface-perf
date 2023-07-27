# `c-interface-perf`: A comparison of performance for various "interface" implementations in C (and C++).

> **:warning: NOTE:** This codebase is still being actively worked on. At some point this repository will be considered "complete" and this note will be removed. At that point any takeaways, if any, can be considered finalized.

## Summary

Inspired by the conversation of interface design and how to achieve it with zig, this repository implements three methods of interface design in C (and one method in C++).
The problem of implementing a psuedo-random number generator interface is examined similar to what happened in the zig discussions.
Two implementations of the psuedo-random number generator interface based off the [xorshift](https://en.wikipedia.org/wiki/Xorshift) family of psuedo-random algorithms were implemented.
The [xoshiross](https://en.wikipedia.org/wiki/Xorshift#xoshiro256**) algorithm and the [xoshirop](https://en.wikipedia.org/wiki/Xorshift#xoshiro256+) algorithm were chosen and implemented with four different styles of interface design.
An informal benchmark of each interface implementation is done for four use cases:

* Looping over a large array/collection of interface objects where the objects are randomly chosen with replacement. The `random` method is called once for each interface object. In the default case there are 16 instantiations for each of the two algorithms resulting in 32 different interface objects to choose from. This test varies the type of backing implementation of the interface as well as the instantiations of the implementations.

* Looping over a large array/collection of interface objects where the objects are randomly chosen with replacement from only one type of implementation. The `random` method is called once for each interface object. In the default case objects are chosen from 16 instantiations of the `xoshiross` implementation. The test fixes the type of the backing implementation but still varies the instantiations of the backing implementation that are used.

* One type of backing implementation is chosen at random and the `random` method is called many times. This randomizes the backing implementation (in hopes of inhibiting the compiler) while fixing which instantiation is used for the test.

* The backing implementation is fixed to be the `xoshiross` implementation and the `random` method is called many times. This fixes the backing implementation as well as fixing which instaniation is used for the test. The compiler should be able to maximally optimize this case as all type info and allocation info is known at compile time.

## Installation

TODO

## Running the Benchmark

TODO

## Comments On Results

TODO
