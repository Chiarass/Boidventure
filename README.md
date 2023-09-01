# Boidventure
Boid project for unibo's physics course. 
to build in release mode, as is advised for better performance:

```
cmake -S ./ -B build/release -DBUILD_TESTING=True -DCMAKE_BUILD_TYPE=Release
```
and then 

```
cmake --build build/release
```
to run the program:
```
./build/release/boids
```
to run the test for the program:
```
./build/release/boids.t
```

To build in debug mode use instead:
```
cmake -S ./ -B build/debug -DBUILD_TESTING=True -DCMAKE_BUILD_TYPE=Debug
```

and then 

```
cmake --build build/debug
```
to run the program:
```
./build/debug/boids
```
to run the test for the program:
```
./build/debug/boids.t
```
