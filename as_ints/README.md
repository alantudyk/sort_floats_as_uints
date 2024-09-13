### i3-8109U, Ubuntu 24.04:
```
$ ./test.sh

gcc:

	as floats:      133 ms
	as   ints:      126 ms

clang:

	as floats:      117 ms
	as   ints:      110 ms

```
### Snapdragon 778G:
```
$ ./test

	as floats:      138 ms
	as   ints:      126 ms

$ taskset -c 0 ./test

	as floats:      301 ms
	as   ints:      293 ms

```
