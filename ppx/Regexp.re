let variable = Re.Pcre.regexp("\\{(\\w+)}");

let findAll = (~regexp, s) =>
  Re.all(regexp, s)
  /* every match implicitly has a group 0 that covers the whole match, */
  /* and explicit groups are numbered from 1 */
  |> List.map(Re.Group.get(_, 1))
  |> List.sort_uniq(String.compare);

let extractTagNames = s => {
  let openTagPattern = Re.Pcre.regexp("<(\\w+)");
  let closeTagPattern = Re.Pcre.regexp("</(\\w+)>");

  let openTags = findAll(~regexp=openTagPattern, s);
  let closeTags = findAll(~regexp=closeTagPattern, s);

  openTags |> List.filter(tag => List.mem(tag, closeTags)) |> List.sort_uniq(String.compare);
};
