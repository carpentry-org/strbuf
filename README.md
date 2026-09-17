# strbuf

A growable string buffer for Carp.

## Installation

```clojure
(load "git@github.com:carpentry-org/strbuf@0.3.0")
```

## Usage

`StringBuf` provides amortized O(1) appends, unlike `String.append` which
allocates a new string on every call.

```clojure
(let [sb (StringBuf.create)]
  (do
    (StringBuf.append-str &sb "HTTP/1.1 ")
    (StringBuf.append-int &sb 200)
    (StringBuf.append-str &sb " OK")
    (StringBuf.append-crlf &sb)
    (println* &(StringBuf.str &sb))))
```

## Ownership

Buffers follow Carp's memory model like any other value. An owned `StringBuf`
is deleted when it goes out of scope, no matter where it lives: in a `let`, in
an array, inside a struct or sum type, returned from a function, or moved into
one. `@` copies a buffer, and the copy is freed independently. You never call
`StringBuf.delete` yourself; it is there because it implements the `delete`
interface that the compiler calls for you.

Three functions hand you a `String`, and they differ in what happens to the
buffer:

- `str` copies the contents and leaves the buffer untouched
- `to-string` copies the contents and resets the buffer for reuse
- `into-string` consumes the buffer and hands its allocation to the `String`,
  so nothing is copied and nothing is freed

Use `into-string` when you build a string once and are done with the buffer,
and `to-string` when you keep appending to the same buffer afterwards.

```clojure
(defn greeting [name]
  (let-do [sb (StringBuf.create)]
    (StringBuf.append-str &sb "hello, ")
    (StringBuf.append-str &sb name)
    (StringBuf.into-string sb)))
```

## API

- `StringBuf.create` / `StringBuf.with-capacity` — constructors
- `StringBuf.append-str` — append a string
- `StringBuf.append-char` — append a single character
- `StringBuf.append-bytes` — append raw bytes
- `StringBuf.append-int` — append integer as decimal
- `StringBuf.append-long` — append long as decimal
- `StringBuf.append-double` / `StringBuf.append-float` — append as string
- `StringBuf.append-bool` — append `true` or `false`
- `StringBuf.append-crlf` — append `\r\n`
- `StringBuf.length` — current byte count
- `StringBuf.str` / `StringBuf.prn` — copy as String, keep the buffer as is
- `StringBuf.to-string` — copy as String, reset the buffer
- `StringBuf.into-string` — consume the buffer, no copy
- `StringBuf.clear` — reset without freeing

## Testing

```
carp -x --log-memory test/strbuf.carp
```

<hr/>

Have fun!
