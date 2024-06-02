# Throughput — Counter

## Simple throughput counter 
This app calculates throughput dl and ul for two users on Rlc level

### Reqs
g++, ns-3, waf, cmake 

### Running
#### Run scenario
from directory /throughput-counter/*your-ns-allinone*/ns-3.x
> ./waf --run scratch/lte-scenario.cc

#### Run counter
1) Put your files to ./output
2) Follow this commands:
> mkdir build

> cd build
 
> cmake ../

> make

> ./throughput_counter