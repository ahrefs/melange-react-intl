let messageWithMaxLengthExceeding: ReactIntl.message = [%intl
  {
    msg: "i am message with maxLength exceeding",
    desc: "example description",
    maxLength: 20,
  }
];
