/* Finds ICU plural arguments such as
     {count, plural, one {{countStr} item} other {{countStr} items}}

   A branch body can hold any text, including digits and characters like . , ! and '.
   Braces in a body must either belong to a {placeholder} or a nested plural, or be
   ICU-quoted ('{' and '}'). The scanner tracks brace depth and skips quoted text to find
   where each body ends.

   Selectors must be zero, one, few, many and other, in that order, each at most once,
   and other is required. A plural that breaks these rules or isn't closed raises [Error]. */

type t = {
  name: string,
  branches: list(string),
  /* index of the opening brace */
  start: int,
  /* index just past the closing brace */
  stop: int,
};

let isSpace = c => c == ' ' || c == '\t' || c == '\n' || c == '\r';

let isWordChar =
  fun
  | 'a' .. 'z'
  | 'A' .. 'Z'
  | '0' .. '9'
  | '_' => true
  | _ => false;

let skipSpaces = (s, i) => {
  let len = String.length(s);
  let rec go = i => i < len && isSpace(s.[i]) ? go(i + 1) : i;
  go(i);
};

let readWord = (s, i) => {
  let len = String.length(s);
  let rec go = j => j < len && isWordChar(s.[j]) ? go(j + 1) : j;
  let j = go(i);
  j > i ? Some((String.sub(s, i, j - i), j)) : None;
};

let expectChar = (s, i, c) => i < String.length(s) && s.[i] == c ? Some(i + 1) : None;

/* [j] points at an apostrophe. Returns the index just past the ICU quoting construct
   that starts there: [''] is a literal apostrophe, and an apostrophe before [{], [}], [#]
   or [|] quotes everything up to the next lone apostrophe. Any other apostrophe, as in
   "You've", is literal. */
let skipQuote = (s, j) => {
  let len = String.length(s);
  let at = k => k < len ? Some(s.[k]) : None;
  switch (at(j + 1)) {
  | Some('\'') => j + 2
  | Some('{' | '}' | '#' | '|') =>
    let rec go = k =>
      switch (at(k), at(k + 1)) {
      | (None, _) => len
      | (Some('\''), Some('\'')) => go(k + 2)
      | (Some('\''), _) => k + 1
      | _ => go(k + 1)
      };
    go(j + 2);
  | _ => j + 1
  };
};

/* [i] points at an opening brace. Returns the index of the matching closing brace. */
let findClosingBrace = (s, i) => {
  let len = String.length(s);
  let rec go = (j, depth) =>
    if (j >= len) {
      None;
    } else {
      switch (s.[j]) {
      | '\'' => go(skipQuote(s, j), depth)
      | '{' => go(j + 1, depth + 1)
      | '}' when depth == 1 => Some(j)
      | '}' => go(j + 1, depth - 1)
      | _ => go(j + 1, depth)
      };
    };
  go(i, 0);
};

let ( let* ) = Option.bind;

/* Raised for a message that starts a plural ([{name, plural,]) but doesn't finish it. */
exception Error(string);

/* The selectors a plural may use, in the order they must appear. [other] is required. */
let selectors = ["zero", "one", "few", "many", "other"];

let rec indexOf = (x, ~from=0) =>
  fun
  | [] => None
  | [y, ..._] when y == x => Some(from)
  | [_, ...rest] => indexOf(x, ~from=from + 1, rest);

/* Parses [{name, plural, selector {body} ... other {body}}] starting at the opening brace [start].
   Returns [None] when the text there isn't a plural header. Raises [Error] when it is one but
   the rest is malformed or uses selectors other than [selectors], in that order. */
let parseAt = (s, start) => {
  let* i = expectChar(s, start, '{');
  let* (name, i) = readWord(s, skipSpaces(s, i));
  let* i = expectChar(s, skipSpaces(s, i), ',');
  let* (kind, i) = readWord(s, skipSpaces(s, i));
  let* i = kind == "plural" ? Some(i) : None;
  let* i = expectChar(s, skipSpaces(s, i), ',');
  let fail = reason => raise(Error(Printf.sprintf("plural `%s` %s", name, reason)));
  /* [previous] is the selector of the branch before, if any. */
  let rec branches = (i, acc, previous) => {
    let i = skipSpaces(s, i);
    if (i >= String.length(s)) {
      fail("is not closed");
    };
    switch (expectChar(s, i, '}')) {
    | Some(stop) =>
      if (previous != Some("other")) {
        fail("has no `other` branch");
      };
      {
        name,
        branches: List.rev(acc),
        start,
        stop,
      };
    | None =>
      let (selector, i) =
        switch (readWord(s, i)) {
        | Some(found) => found
        | None => fail(Printf.sprintf("has unexpected `%c` where a selector should be", s.[i]))
        };
      let rank =
        switch (indexOf(selector, selectors)) {
        | Some(rank) => rank
        | None =>
          fail(Printf.sprintf("has unknown selector `%s`; use %s", selector, String.concat(", ", selectors)))
        };
      switch (previous) {
      | Some(prev) when prev == selector => fail(Printf.sprintf("repeats selector `%s`", selector))
      | Some(prev) when Option.get(indexOf(prev, selectors)) >= rank =>
        fail(
          Printf.sprintf(
            "has selector `%s` after `%s`; selectors go in the order %s",
            selector,
            prev,
            String.concat(", ", selectors),
          ),
        )
      | _ => ()
      };
      let i = skipSpaces(s, i);
      if (expectChar(s, i, '{') == None) {
        fail(Printf.sprintf("is missing `{` after selector `%s`", selector));
      };
      let close =
        switch (findClosingBrace(s, i)) {
        | Some(close) => close
        | None => fail(Printf.sprintf("has an unclosed `%s` branch", selector))
        };
      let body = String.sub(s, i + 1, close - i - 1);
      branches(close + 1, [body, ...acc], Some(selector));
    };
  };
  Some(branches(i, [], None));
};

/* Top-level plurals in [s], left to right. */
let findAll = s => {
  let len = String.length(s);
  let rec go = (i, acc) =>
    if (i >= len) {
      List.rev(acc);
    } else if (s.[i] == '\'') {
      go(skipQuote(s, i), acc);
    } else if (s.[i] == '{') {
      switch (parseAt(s, i)) {
      | Some(plural) => go(plural.stop, [plural, ...acc])
      | None => go(i + 1, acc)
      };
    } else {
      go(i + 1, acc);
    };
  go(0, []);
};

/* Splits a message into the names of its plural arguments (nested ones included) and
   the remaining text: everything outside the plurals followed by every branch body,
   with the plural syntax itself removed. Callers look for placeholders and tags in the
   remaining text. */
let rec extract = s => {
  let plurals = findAll(s);
  let (outside, last) =
    List.fold_left(
      ((parts, pos), plural) => ([String.sub(s, pos, plural.start - pos), ...parts], plural.stop),
      ([], 0),
      plurals,
    );
  let outside = [String.sub(s, last, String.length(s) - last), ...outside] |> List.rev;
  let nested = plurals |> List.concat_map(plural => plural.branches) |> List.map(extract);
  let names = List.map(plural => plural.name, plurals) @ List.concat_map(fst, nested);
  let text = String.concat(" ", outside @ List.map(snd, nested));
  (List.sort_uniq(String.compare, names), text);
};
