module ReactIntlPpxAdaptor = {
  module Message = {
    let to_s = (message: ReactIntl.message): string => message.defaultMessage;
    let format_to_s =
        (
          ~list_of_values as _: list((string, ReactIntl.value)),
          message: ReactIntl.message,
          _values: Js.t({..}),
        )
        : string =>
      message.defaultMessage;
    let format_to_el =
        (
          ~list_of_values as _: list((string, ReactIntl.value)),
          message: ReactIntl.message,
          _values: Js.t({..}),
        )
        : React.element =>
      React.string(message.defaultMessage);
  };
};

let message: ReactIntl.message = [%intl "i am message"];
let descriptedMessage: ReactIntl.message = [%intl
  {msg: "i am message", desc: "i am description"}
];

let message2: string = [%intl.s "blabla"];
let descriptedmessage2: string = [%intl.s
  {msg: "i am message", desc: "i am description"}
];

let element3: React.element = [%intl.el "blabla"];
let descriptedElement3: React.element = [%intl.el
  {msg: "i am message", desc: "i am description"}
];

let stringWithVariable: {. "variable": string} => string = [%intl.s
  "I am string with {variable}"
];

let stringWithPluralForm: {. "itemsCount": int} => string = [%intl.s
  "{itemsCount, plural, zero {item} one {item} few {items} many {items} other {items}}"
];

let elementWithVariable: {. "variable": React.element} => React.element = [%intl.el
  "I am react element with {variable}"
];

let elementWithPluralForm: {. "itemsCount": int} => React.element = [%intl.el
  "{itemsCount, plural, zero {item} one {item} few {items} many {items} other {items}}"
];

let elementWithRichText: {. "a": string => React.element} => React.element = [%intl.el
  "Some text with <a>link text</a>"
];
