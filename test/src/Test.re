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

let message: ReactIntl.message = [%intl "blabla"];
let descriptedMessage: ReactIntl.message = [%intl {msg: "blabla", desc: "i am description"}]

let message2: string = [%intl.s "blabla"];
let descriptedmessage2: string = [%intl.s {msg: "blabla", desc: "i am description"}];

let element3: React.element = [%intl.el "blabla"];
let descriptedElement3: React.element = [%intl.el {msg: "blabla", desc: "i am description"}];
