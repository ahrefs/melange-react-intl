/* Finds ICU plural arguments such as
     {count, plural, one {{countStr} item} other {{countStr} items}}
   Branch bodies may contain placeholders, punctuation and nested plurals, so the
   scanner tracks brace depth instead of matching the body with a regexp. */

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

/* [i] points at an opening brace. Returns the index of the matching closing brace. */
let findClosingBrace = (s, i) => {
  let len = String.length(s);
  let rec go = (j, depth) =>
    if (j >= len) {
      None;
    } else {
      switch (s.[j]) {
      | '{' => go(j + 1, depth + 1)
      | '}' when depth == 1 => Some(j)
      | '}' => go(j + 1, depth - 1)
      | _ => go(j + 1, depth)
      };
    };
  go(i, 0);
};

/* Selectors are keywords (zero, one, two, few, many, other) or exact matches (=0). */
let readSelector = (s, i) =>
  switch (expectChar(s, i, '=')) {
  | Some(j) =>
    switch (readWord(s, j)) {
    | Some((n, k)) => Some(("=" ++ n, k))
    | None => None
    }
  | None => readWord(s, i)
  };

let ( let* ) = Option.bind;

/* Parses [{name, plural, selector {body} ... other {body}}] starting at the opening brace [start]. */
let parseAt = (s, start) => {
  let* i = expectChar(s, start, '{');
  let* (name, i) = readWord(s, skipSpaces(s, i));
  let* i = expectChar(s, skipSpaces(s, i), ',');
  let* (kind, i) = readWord(s, skipSpaces(s, i));
  let* i = kind == "plural" ? Some(i) : None;
  let* i = expectChar(s, skipSpaces(s, i), ',');
  let rec branches = (i, acc, hasOther) => {
    let i = skipSpaces(s, i);
    switch (expectChar(s, i, '}')) {
    | Some(stop) =>
      hasOther
        ? Some({
            name,
            branches: List.rev(acc),
            start,
            stop,
          })
        : None
    | None =>
      let* (selector, i) = readSelector(s, i);
      let i = skipSpaces(s, i);
      let* _ = expectChar(s, i, '{');
      let* close = findClosingBrace(s, i);
      let body = String.sub(s, i + 1, close - i - 1);
      branches(close + 1, [body, ...acc], hasOther || selector == "other");
    };
  };
  branches(i, [], false);
};

/* Top-level plurals in [s], left to right. */
let findAll = s => {
  let len = String.length(s);
  let rec go = (i, acc) =>
    if (i >= len) {
      List.rev(acc);
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
