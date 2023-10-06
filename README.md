This is a coding test for a company interview done in 2023.

I unfortunately did not secure a follow-up, due to the code being auto rejected for
using unsupported operators on `K` or `V`. The example the coding test page gave
mentioned using the default constructors of `m_map[key] = value;`.
See the comments in `main.cpp` for a description of the limited operators on `K`` and `V``.

Anyways, the overall problem is to implement the `assign()` function for the
`interval_map<K,V>` class, which is built on top of a `std::map<K,V> m_map` and `V m_valBegin`.

The `interval_map`, when constructed, associates value `V` for all values of `K`.

For example, constructing `interval_map<int, char>('A')` results in `m_map` being empty, `m_valBegin = 'A'` and gives:
```
 ...
-1 -> 'A'
 0 -> 'A'
 1 -> 'A'
 2 -> 'A'
 3 -> 'A'
 4 -> 'A'
 5 -> 'A'
 ...
```

Calling `assign(2, 4, 'B')` on the constructed `interval_map` results in m_map containing entries:
```
 (2, B)
 (4, A)
```
and  gives:
```
 ...
-1 -> 'A'
 0 -> 'A'
 1 -> 'A'
 2 -> 'B'
 3 -> 'B'
 4 -> 'A'
 5 -> 'A'
 ...
```
