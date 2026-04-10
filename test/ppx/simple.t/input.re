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
