# malloc/free counter based on LD\_PRELOAD

Usage:
```
./instrument.sh /path/to/a/binary/you/wish/to/inspect
```

Example:
```
[sarna@localhost scylla-rust-driver]$ ~/repo/trace_alloc/instrument.sh target/release/examples/parallel-prepared
g++ -O3 -shared -fPIC preload.cc -o preload.so
Connecting to 127.0.0.1:9042 ...
0
10000
20000
30000
40000
50000
60000
70000
80000
90000
Ok.
----------
Alloc stats
----------
Mallocs:        968834
Reallocs:       781169
Callocs:         98592
----------
sum =          1848595
----------
Frees:         1089737

```
