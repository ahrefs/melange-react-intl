let variable = Re.Pcre.regexp("\\{(\\w+)}");

let plural =
  Re.Pcre.regexp(
    "\\{(\\w+), plural, zero \\{[^\\}]+\\} one \\{[^\\}]+\\} few \\{[^\\}]+\\}(?: many \\{[^\\}]+\\})? other \\{[A-Za-z ]+\\}\\}",
  );
let richText = Re.Pcre.regexp("<(\\w+)>[^<]+</\\w+>");

let findAll = (~regexp, s) =>
  Re.all(regexp, s)
  /* every match implicitly has a group 0 that covers the whole match, */
  /* and explicit groups are numbered from 1 */
  |> List.map(Re.Group.get(_, 1))
  |> List.sort_uniq(String.compare);

let remove = (~regexp, s) => Re.replace_string(~all=true, regexp, ~by="", s);
