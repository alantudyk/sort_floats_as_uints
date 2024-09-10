### i3-8109U, Ubuntu 24.04:
```
$ ./test.sh

gcc:

	as floats:      122 ms
	as  uints:      108 ms

clang:

	as floats:      118 ms
	as  uints:      109 ms

```
### Snapdragon 778G:
```
$ ./test

	as floats:      137 ms
	as  uints:      125 ms

$ taskset -c 0 ./test

	as floats:      300 ms
	as  uints:      295 ms

```
