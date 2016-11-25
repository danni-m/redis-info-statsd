# Redis Statsd Module
The purpose of this module is to get performance stats from redis.

## Overview
The module creates a hiredis client and a statsd client.

## known issues
1. Some `info` parmas are not parsed currectly.
2. statsd host and port are not configurable
3. update interval is not configurable
4. statsd namesapce is not configurable

## License: AGPL
Which basically means you can freely use this for your own projects without "virality" to your code, as long as you're not modifying the module itself.

# How to start using?
## Compile
### dependancies
* cmake
* pthreads
* hiredis (install via brew or your favourite package manager)
* statsd-c-client (fetch using git submodule)
* RedisModulesSDK ()
## Compile
1. fetch submodules, run the following:
~~~~
git submodule init
git submodule update
~~~~
2. compile RedisModulesSDK:
```{r, engine='bash', count_lines}
cd RedisModulesSDK/rmutil
make
```
3. go back to main directory, and create a `bin` directory, run cmake and compile:
```{r, engine='bash', count_lines}
cd ../..
mkdir bin
cd bin
cmake ..
make
```
the module should be in your current directory (`bin`) named `libredis_info_statsd.dylib` or `libredis_info_statsd.so`.
## load module
using the loadmodule command, or by configuration file:
`loadmodule PATH_TO_redis-info-statsd/bin/libredis_info_statsd.dylib`