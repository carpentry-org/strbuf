# strbuf

A growable string buffer for Carp.

## Installation

```clojure
(load "git@github.com:carpentry-org/strbuf@0.1.0")
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
    (println* &(StringBuf.to-string &sb))
    (StringBuf.delete sb)))
```

`to-string` returns the contents and resets the buffer for reuse.
`str` copies the contents without resetting.

## API

- `StringBuf.create` / `StringBuf.with-capacity` — constructors
- `StringBuf.append-str` — append a string
- `StringBuf.append-char` — append a single character
- `StringBuf.append-bytes` — append raw bytes
- `StringBuf.append-int` — append integer as decimal
- `StringBuf.append-double` — append double as string
- `StringBuf.append-crlf` — append `\r\n`
- `StringBuf.length` — current byte count
- `StringBuf.to-string` — extract as String, reset buffer
- `StringBuf.str` — copy as String, keep buffer
- `StringBuf.clear` — reset without freeing
- `StringBuf.delete` — free the buffer

## Testing

```
carp -x test/strbuf.carp
```

<hr/>

Have fun!
