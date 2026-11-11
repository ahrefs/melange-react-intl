Plural without an other branch

  $ cat > input.re <<EOF
  > let t = [%intl.s "{count, plural, one {item}}"];
  > EOF

  $ ./ppx.sh --output re input.re
  File "output.ml", line 1, characters 17-46:
  1 | let t = [%intl.s "{count, plural, one {item}}"]
                       ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
  Error: melange-react-intl: plural `count` has no `other` branch
  [1]

Plural with an unclosed branch

  $ cat > input.re <<EOF
  > let t = [%intl.s "{count, plural, one {item} other {{countStr} items}"];
  > EOF

  $ ./ppx.sh --output re input.re
  File "output.ml", line 1, characters 17-70:
  1 | let t = [%intl.s "{count, plural, one {item} other {{countStr} items}"]
                       ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
  Error: melange-react-intl: plural `count` is not closed
  [1]

Plural with a missing branch body

  $ cat > input.re <<EOF
  > let t = [%intl.el "{count, plural, one item other {items}}"];
  > EOF

  $ ./ppx.sh --output re input.re
  File "output.ml", line 1, characters 18-59:
  1 | let t = [%intl.el "{count, plural, one item other {items}}"]
                        ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
  Error: melange-react-intl: plural `count` is missing `{` after selector `one`
  [1]

Exact-match selectors are not supported

  $ cat > input.re <<EOF
  > let t = [%intl.s "{count, plural, =0 {No items} other {items}}"];
  > EOF

  $ ./ppx.sh --output re input.re
  File "output.ml", line 1, characters 17-63:
  1 | let t = [%intl.s "{count, plural, =0 {No items} other {items}}"]
                       ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
  Error: melange-react-intl: plural `count` has unexpected `=` where a selector should be
  [1]

Unknown selector

  $ cat > input.re <<EOF
  > let t = [%intl.s "{count, plural, one {item} two {items} other {items}}"];
  > EOF

  $ ./ppx.sh --output re input.re
  File "output.ml", line 1, characters 17-72:
  1 | let t = [%intl.s "{count, plural, one {item} two {items} other {items}}"]
                       ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
  Error: melange-react-intl: plural `count` has unknown selector `two`; use zero, one, few, many, other
  [1]

Selectors out of order

  $ cat > input.re <<EOF
  > let t = [%intl.s "{count, plural, other {items} one {item}}"];
  > EOF

  $ ./ppx.sh --output re input.re
  File "output.ml", line 1, characters 17-60:
  1 | let t = [%intl.s "{count, plural, other {items} one {item}}"]
                       ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
  Error: melange-react-intl: plural `count` has selector `one` after `other`; selectors go in the order zero, one, few, many, other
  [1]

Repeated selector

  $ cat > input.re <<EOF
  > let t = [%intl.s "{count, plural, one {item} one {thing} other {items}}"];
  > EOF

  $ ./ppx.sh --output re input.re
  File "output.ml", line 1, characters 17-72:
  1 | let t = [%intl.s "{count, plural, one {item} one {thing} other {items}}"]
                       ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
  Error: melange-react-intl: plural `count` repeats selector `one`
  [1]

Plural argument reused as a placeholder

  $ cat > input.re <<EOF
  > let t = [%intl.s "{count, plural, one {{count} item} other {{count} items}}"];
  > EOF

  $ ./ppx.sh --output re input.re
  File "output.ml", line 1, characters 17-76:
  1 | let t = [%intl.s "{count, plural, one {{count} item} other {{count} items}}"]
                       ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
  Error: melange-react-intl: plural `count` is also used as the placeholder `{count}`; pass the formatted number as a separate variable instead
  [1]

Plural argument reused as a placeholder outside the plural

  $ cat > input.re <<EOF
  > let t = [%intl.s "{count} {count, plural, one {item} other {items}}"];
  > EOF

  $ ./ppx.sh --output re input.re
  File "output.ml", line 1, characters 17-68:
  1 | let t = [%intl.s "{count} {count, plural, one {item} other {items}}"]
                       ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
  Error: melange-react-intl: plural `count` is also used as the placeholder `{count}`; pass the formatted number as a separate variable instead
  [1]

Unquoted # in a branch

  $ cat > input.re <<EOF
  > let t = [%intl.s "{count, plural, one {# item} other {# items}}"];
  > EOF

  $ ./ppx.sh --output re input.re
  File "output.ml", line 1, characters 17-64:
  1 | let t = [%intl.s "{count, plural, one {# item} other {# items}}"]
                       ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
  Error: melange-react-intl: plural `count` uses `#` in its `one` branch; pass the formatted count as a separate variable instead
  [1]

Plain intl is checked too

  $ cat > input.re <<EOF
  > let t = [%intl "{count, plural, one {item}}"];
  > EOF

  $ ./ppx.sh --output re input.re
  File "output.ml", line 1, characters 15-44:
  1 | let t = [%intl "{count, plural, one {item}}"]
                     ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
  Error: melange-react-intl: plural `count` has no `other` branch
  [1]

Nested plural

  $ cat > input.re <<EOF
  > let t = [%intl.s "{a, plural, one {x} other {{b, plural, one {y} other {z}}}}"];
  > EOF

  $ ./ppx.sh --output re input.re
  File "output.ml", line 2, characters 11-72:
  2 |   [%intl.s "{a, plural, one {x} other {{b, plural, one {y} other {z}}}}"]
                 ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
  Error: melange-react-intl: plural `a` contains nested plural `b`
  [1]

Nested plural that is itself malformed reports the nesting

  $ cat > input.re <<EOF
  > let t = [%intl.s "{a, plural, one {{b, plural, one {x}}} other {y}}"];
  > EOF

  $ ./ppx.sh --output re input.re
  File "output.ml", line 1, characters 17-68:
  1 | let t = [%intl.s "{a, plural, one {{b, plural, one {x}}} other {y}}"]
                       ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
  Error: melange-react-intl: plural `a` contains nested plural `b`
  [1]
