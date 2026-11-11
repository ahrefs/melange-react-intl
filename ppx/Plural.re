/* Finds ICU plural arguments such as
     {count, plural, one {{countStr} item} other {{countStr} items}}

   A branch body can hold any text, including digits and characters like . , ! and '.
   Braces in a body must either belong to a {placeholder} or be ICU-quoted ('{' and '}').
   The scanner tracks brace depth and skips quoted text to find where each body ends.

   Selectors must be zero, one, few, many and other, in that order, each at most once,
   and other is required. A body can't contain another plural or an unquoted #, and the
   plural argument can't also appear as a {placeholder}. Breaking any of these rules, or
   leaving the plural unclosed, raises [Error]. */

type t = {
  name: string,
  branches: list(string),
  /* index of the opening brace */
  start: int,
  /* index just past the closing brace */
  stop: int,
};

/* Raised when a message breaks one of the rules above. The string says how. */
exception Error(string);

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
   that starts there, or [None] when the apostrophe is literal (as in "You've").
   [''] is a literal apostrophe. An apostrophe before {, }, < or >, or before # inside a
   plural branch, quotes everything up to the next lone apostrophe. These are the
   characters formatjs treats as quotable. */
let quoteEnd = (~inPlural, s, j) => {
  let len = String.length(s);
  let at = k => k < len ? Some(s.[k]) : None;
  /* Index just past the lone apostrophe that closes a quote; [''] inside stays quoted. */
  let rec closeAt = k =>
    switch (at(k), at(k + 1)) {
    | (None, _) => len
    | (Some('\''), Some('\'')) => closeAt(k + 2)
    | (Some('\''), _) => k + 1
    | _ => closeAt(k + 1)
    };
  switch (at(j + 1)) {
  | Some('\'') => Some(j + 2)
  | Some('{' | '}' | '<' | '>') => Some(closeAt(j + 2))
  | Some('#') when inPlural => Some(closeAt(j + 2))
  | _ => None
  };
};

/* [s] with every quoting construct replaced by a space, so later scans for {placeholders},
   <tags> and # only see unquoted text. */
let blankQuoted = (~inPlural, s) => {
  let len = String.length(s);
  let buf = Buffer.create(len);
  let rec go = i =>
    if (i < len) {
      switch (s.[i] == '\'' ? quoteEnd(~inPlural, s, i) : None) {
      | Some(next) =>
        Buffer.add_char(buf, ' ');
        go(next);
      | None =>
        Buffer.add_char(buf, s.[i]);
        go(i + 1);
      };
    };
  go(0);
  Buffer.contents(buf);
};

/* [i] points at the opening brace of a branch body. Returns the index of the matching
   closing brace. */
let findClosingBrace = (s, i) => {
  let len = String.length(s);
  let rec go = (j, depth) =>
    if (j >= len) {
      None;
    } else {
      switch (s.[j]) {
      | '\'' => go(Option.value(quoteEnd(~inPlural=true, s, j), ~default=j + 1), depth)
      | '{' => go(j + 1, depth + 1)
      | '}' when depth == 1 => Some(j)
      | '}' => go(j + 1, depth - 1)
      | _ => go(j + 1, depth)
      };
    };
  go(i, 0);
};

let ( let* ) = Option.bind;

/* The selectors a plural may use, in the order they must appear. [other] is required. */
let selectors = ["zero", "one", "few", "many", "other"];

/* Reads [{name, plural,] starting at the opening brace [start]. Returns the name and the index
   just past the second comma. */
let parseHeader = (s, start) => {
  let* i = expectChar(s, start, '{');
  let* (name, i) = readWord(s, skipSpaces(s, i));
  let* i = expectChar(s, skipSpaces(s, i), ',');
  let* (kind, i) = readWord(s, skipSpaces(s, i));
  let* i = kind == "plural" ? Some(i) : None;
  let* i = expectChar(s, skipSpaces(s, i), ',');
  Some((name, i));
};

/* Name of the first plural header in [s], which must already have quoted text blanked. */
let findHeader = s => {
  let len = String.length(s);
  let rec go = i =>
    if (i >= len) {
      None;
    } else {
      switch (s.[i] == '{' ? parseHeader(s, i) : None) {
      | Some((name, _)) => Some(name)
      | None => go(i + 1)
      };
    };
  go(0);
};

/* Parses [{name, plural, selector {body} ... other {body}}] starting at the opening brace [start].
   Returns [None] when the text there isn't a plural header. Raises [Error] when it is one but
   breaks a rule from the top of this file. Branch bodies come back with quoted text blanked. */
let parseAt = (s, start) => {
  let* (name, i) = parseHeader(s, start);
  let fail = reason => raise(Error(Printf.sprintf("plural `%s` %s", name, reason)));
  /* [previous] is the selector of the branch before and its position in [selectors]. */
  let rec branches = (i, acc, previous) => {
    let i = skipSpaces(s, i);
    if (i >= String.length(s)) {
      fail("is not closed");
    };
    switch (expectChar(s, i, '}')) {
    | Some(stop) =>
      switch (previous) {
      | Some(("other", _)) => ()
      | _ => fail("has no `other` branch")
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
        switch (List.find_index(String.equal(selector), selectors)) {
        | Some(rank) => rank
        | None =>
          fail(Printf.sprintf("has unknown selector `%s`; use %s", selector, String.concat(", ", selectors)))
        };
      switch (previous) {
      | Some((prev, _)) when prev == selector => fail(Printf.sprintf("repeats selector `%s`", selector))
      | Some((prev, prevRank)) when prevRank >= rank =>
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
      let body = String.sub(s, i + 1, close - i - 1) |> blankQuoted(~inPlural=true);
      switch (findHeader(body)) {
      | Some(inner) => fail(Printf.sprintf("contains nested plural `%s`", inner))
      | None => ()
      };
      if (String.contains(body, '#')) {
        fail(
          Printf.sprintf(
            "uses `#` in its `%s` branch; pass the formatted count as a separate variable instead",
            selector,
          ),
        );
      };
      branches(close + 1, [body, ...acc], Some((selector, rank)));
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
      go(Option.value(quoteEnd(~inPlural=false, s, i), ~default=i + 1), acc);
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

/* Splits a message into the names of its plural arguments and the remaining text:
   everything outside the plurals followed by every branch body, with the plural syntax
   removed and quoted text blanked. Callers look for placeholders and tags in the
   remaining text, so quoted '{name}' or '<b>' is never taken for one. */
let extract = s => {
  let plurals = findAll(s);
  let (outside, last) =
    List.fold_left(
      ((parts, pos), plural) => ([String.sub(s, pos, plural.start - pos), ...parts], plural.stop),
      ([], 0),
      plurals,
    );
  let outside =
    [String.sub(s, last, String.length(s) - last), ...outside]
    |> List.rev
    |> List.map(blankQuoted(~inPlural=false));
  let bodies = List.concat_map(plural => plural.branches, plurals);
  let names = List.map(plural => plural.name, plurals) |> List.sort_uniq(String.compare);
  let text = String.concat(" ", outside @ bodies);
  let placeholders = Regexp.findAll(~regexp=Regexp.variable, text);
  names
  |> List.iter(name =>
       if (List.mem(name, placeholders)) {
         raise(
           Error(
             Printf.sprintf(
               "plural `%s` is also used as the placeholder `{%s}`; pass the formatted number as a separate variable instead",
               name,
               name,
             ),
           ),
         );
       }
     );
  (names, text);
};
