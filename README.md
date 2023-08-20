# Final project: boids
Boid project for unibo's physics course. 
to compile:
```
cmake -S . -B build/debug -DCMAKE_BUILD_TYPE=Release
```
I will eventually change it to a debug configuration (once i understand how the launch.json file should be configured for cmake projects).

then:

```
cmake --build build/debug
```
and to run the code:
```
./build/debug/nomefile
```
