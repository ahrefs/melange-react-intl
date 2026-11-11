module ReactIntlPpxAdaptor = {
  module Message = {
    let to_s = (message: ReactIntl.message): string => message.defaultMessage;
    let format_to_s =
        (~list_of_values as _: list((string, ReactIntl.value)), message: ReactIntl.message, _values: Js.t({..}))
        : string =>
      message.defaultMessage;
    let format_to_el =
        (~list_of_values as _: list((string, ReactIntl.value)), message: ReactIntl.message, _values: Js.t({..}))
        : React.element =>
      React.string(message.defaultMessage);
  };
};

let message: ReactIntl.message = [%intl "i am message"];
let descriptedMessage: ReactIntl.message = [%intl
  {
    msg: "i am message",
    desc: "i am description",
  }
];

let message2: string = [%intl.s "blabla"];
let descriptedmessage2: string = [%intl.s
  {
    msg: "i am message",
    desc: "i am description",
  }
];

let element3: React.element = [%intl.el "blabla"];
let descriptedElement3: React.element = [%intl.el
  {
    msg: "i am message",
    desc: "i am description",
  }
];

let stringWithVariable: {. "variable": string} => string = [%intl.s "I am string with {variable}"];

let stringWithPluralForm: {. "itemsCount": int} => string = [%intl.s
  "{itemsCount, plural, zero {item} one {item} few {items} many {items} other {items}}"
];

let elementWithVariable: {. "variable": React.element} => React.element = [%intl.el
  "I am react element with {variable}"
];

let elementWithPluralForm: {. "itemsCount": int} => React.element = [%intl.el
  "{itemsCount, plural, zero {item} one {item} few {items} many {items} other {items}}"
];

let stringWithPluralBranchVariable:
  {
    .
    "total": int,
    "totalStr": string,
  } =>
  string = [%intl.s
  "{total, plural, one {Select {totalStr} prompt} other {Select all {totalStr} prompts}}"
];

let stringWithPluralBranchPunctuation:
  {
    .
    "count": int,
    "countStr": string,
    "name": string,
  } =>
  string = [%intl.s
  "{name}: {count, plural, one {You've selected {countStr} item.} other {You've selected {countStr} items, {name}.}}"
];

let stringWithQuotedBracesInPluralBranch:
  {
    .
    "count": int,
    "name": string,
  } =>
  string = [%intl.s
  "{count, plural, one {It''s one '{'item'}' for {name}} other {You've got '}' and '{' for {name}}}"
];

let stringWithSiblingPlurals:
  {
    .
    "apples": int,
    "applesStr": string,
    "pears": int,
    "pearsStr": string,
  } =>
  string = [%intl.s
  "{apples, plural, one {{applesStr} apple} other {{applesStr} apples}} and {pears, plural, one {{pearsStr} pear} other {{pearsStr} pears}}"
];

let elementWithQuotedSyntaxInPluralBranch:
  {
    .
    "n": int,
    "nStr": React.element,
  } =>
  React.element = [%intl.el
  "{n, plural, one {Type '<b>'x'</b>' or '{name}' as '#'1} other {Type '<b>'xs'</b>' or '{name}' as '#'{nStr}}}"
];

let elementWithPluralBranchVariable:
  {
    .
    "total": int,
    "totalStr": React.element,
  } =>
  React.element = [%intl.el
  "{total, plural, one {Select {totalStr} prompt} other {Select all {totalStr} prompts}}"
];

let elementWithRichText: {. "a": string => React.element} => React.element = [%intl.el
  "Some text with <a>link text</a>"
];
