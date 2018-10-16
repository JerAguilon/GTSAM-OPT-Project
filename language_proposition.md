# Data types
We will support `int`, `bool`, `double`, and `structs`.

# Boolean logic
```
a and b
a or b
```

# Code blocks/scoping
We will use brackets to delimit code blocks:

```
extern fprintf(x)

def my_func(x) {
    x = cool_func_call()
    for (i=0, i < 10, 1.0) {
        fprintf(x)
    }
}
```

Why? It's simpler in implementation to just count brackets rather than tracking whitespace.
Whitespace has many edge cases (do we allow tabs? 2 spaces? 4 spaces? interpret the file
and decide then?).

# Control flows
If statements will be computed like so:
```
if (statement) {

}
```

# Loop control
For for loops, we will allow one variable as this is easier to implement and
likely serves our needs.
```
for (i=0, i < 10, [optional: increment int or double]) {

}
```

While loops can be more flexible:
```
i = 0
j = 1
while (i < 5 and j <  6) {

}
```
