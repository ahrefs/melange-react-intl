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

Nested plural without an other branch

  $ cat > input.re <<EOF
  > let t = [%intl.s "{a, plural, one {{b, plural, one {x}}} other {y}}"];
  > EOF

  $ ./ppx.sh --output re input.re
  File "output.ml", line 1, characters 17-68:
  1 | let t = [%intl.s "{a, plural, one {{b, plural, one {x}}} other {y}}"]
                       ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
  Error: melange-react-intl: plural `b` has no `other` branch
  [1]
