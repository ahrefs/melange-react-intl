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

let undescriptedmessage: string = [%intl.s {msg: "i am message"}];

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

let stringWithPluralFormNoZero: {. "itemsCount": int} => string = [%intl.s
  "{itemsCount, plural, one {item} few {items} many {items} other {items}}"
];

let stringWithPluralFormNoFew: {. "itemsCount": int} => string = [%intl.s
  "{itemsCount, plural, zero {item} one {item} many {items} other {items}}"
];

let stringWithPluralFormNoZeroNoFew: {. "itemsCount": int} => string = [%intl.s
  "{itemsCount, plural, one {item} other {items}}"
];

let stringWithPluralFormNoOne: {. "itemsCount": int} => string = [%intl.s
  "{itemsCount, plural, zero {item} few {items} many {items} other {items}}"
];

let stringWithPluralFormOnlyOther: {. "itemsCount": int} => string = [%intl.s
  "{itemsCount, plural, other {items}}"
];

let stringWithPluralBranchVariable: {
  .
  "total": int,
  "totalStr": string,
} =>
string = [%intl.s
  "{total, plural, one {Select {totalStr} prompt} other {Select all {totalStr} prompts}}"
];

let stringWithPluralBranchPunctuation: {
  .
  "count": int,
  "countStr": string,
  "name": string,
} =>
string = [%intl.s
  "{name}: {count, plural, one {You've selected {countStr} item.} other {You've selected {countStr} items, {name}.}}"
];


let stringWithPluralArgumentInBranch: {
  .
  "count": int,
  "folder": string,
} =>
string = [%intl.s
  "{count, plural, one {{count} item in {folder}} other {{count} items in {folder}}}"
];

let stringWithQuotedBracesInPluralBranch: {
  .
  "count": int,
  "name": string,
} =>
string = [%intl.s
  "{count, plural, one {It''s one '{'item'}' for {name}} other {You've got '}' and '{' for {name}}}"
];

let stringWithSiblingPlurals: {
  .
  "apples": int,
  "applesStr": string,
  "pears": int,
  "pearsStr": string,
} =>
string = [%intl.s
  "{apples, plural, one {{applesStr} apple} other {{applesStr} apples}} and {pears, plural, one {{pearsStr} pear} other {{pearsStr} pears}}"
];

let elementWithPluralBranchVariable: {
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

let cellText = (~powerUsersCount) => {
  [%intl.el
    {js|{powerUsersCountString} {powerUsersCount, plural, zero {Power users} one {Power user} few {Power users} other {Power users}}|js}
  ] @@
  {
    "powerUsersCountString": powerUsersCount->RR.int,
    "powerUsersCount": powerUsersCount,
  };
};
let elementWithNestedTags = [%intl.el
  {js|
  <ul>
    <li>one</li>
    <li>two</li>
  </ul>
  <opentag>
  <p>
    <a href="https://ocaml.com">link text</a>.
  </p>
|js}
];

let messageWithMaxLength: ReactIntl.message = [%intl
  {
    msg: "i am message with maxLength",
    desc: "example description",
    maxLength: 80,
  }
];
