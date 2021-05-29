module ReactIntl = {
  type message = {
    id: string,
    defaultMessage: string,
  };
};

module ReactIntlPpxAdaptor = {
  module Message = {
    let to_s = (message: ReactIntl.message): string => message.defaultMessage;
  };
};

module React = {
  type element = string;
  let string = (localizedString: string): element => localizedString;
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
