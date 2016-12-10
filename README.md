# Redis Statsd Module
The purpose of this module is to get performance stats from redis.

## Overview
The module creates a hiredis client and a statsd client.
On initialization the module creates another thread that will periodically update the statsd server by running the `info` command and than update all corresponding gauges.

## known issues
1. Some `info` parmas are not parsed currectly.
3. update interval is not configurable

## License: AGPL

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
### configuration
By default the module will send the metrics to localhost:8125 using the namespace "redis" you can override it by passing 3 arguments to loadmodule:
`loadmodule PATH_TO_redis-info-stats/bin/libredis_info_statsd.dylib 172.16.0.2 8125 redis-23`