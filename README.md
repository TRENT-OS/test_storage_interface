# Storage interface test

Tests the components that are if_OS_Storage compatible.

General idea is that any storage driver will be tested in this application so
that its compatibility can be proven.

## Getting Started

If a new driver needs to be tested, connect it with the Tester component, and
pass a reference to its interface to the test executors.
