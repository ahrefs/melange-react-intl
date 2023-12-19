open Ppxlib;

let parsePayload = (~loc, payload) =>
  switch (payload) {
  // Match "message"
  | PStr([
      {
        pstr_desc:
          Pstr_eval(
            {pexp_desc: Pexp_constant(Pconst_string(message, _, _)), _} as messageExp,
            _,
          ),
      },
    ]) => (
      message,
      messageExp,
      None,
    )

  // Match {msg: "message", desc: "description"} and {desc: "description", msg: "message"}
  | PStr([
      {
        pstr_desc:
          Pstr_eval(
            {
              pexp_desc:
                Pexp_record(
                  [
                    (
                      {txt: Lident("msg"), _},
                      {
                        pexp_desc:
                          Pexp_constant(Pconst_string(message, _, _)),
                        _,
                      } as messageExp,
                    ),
                    (
                      {txt: Lident("desc"), _},
                      {
                        pexp_desc:
                          Pexp_constant(Pconst_string(description, _, _)),
                        _,
                      },
                    ),
                  ] |
                  [
                    (
                      {txt: Lident("desc"), _},
                      {
                        pexp_desc:
                          Pexp_constant(Pconst_string(description, _, _)),
                        _,
                      },
                    ),
                    (
                      {txt: Lident("msg"), _},
                      {
                        pexp_desc:
                          Pexp_constant(Pconst_string(message, _, _)),
                        _,
                      } as messageExp,
                    ),
                  ],
                  None,
                ),
              _,
            },
            _,
          ),
      },
    ]) => (
      message,
      messageExp,
      Some(description),
    )
  | _ =>
    Location.raise_errorf(
      ~loc,
      "react-intl-ppx expects the extension payload to be a constant string or a record ({msg: string, desc: string}), it does not work with any other expression types.",
    )
  };

let makeId = (~description="", message) =>
  message ++ "|" ++ description |> Digest.string |> Digest.to_hex;

let makeIntlRecord = (~payload, ~loc) => {
  let (message, messageExp, description) = parsePayload(~loc, payload);
  let id = message |> Scanf.unescaped |> makeId(~description?);
  let idExp = Ast_helper.Exp.constant(Pconst_string(id, loc, None));
  %expr
  [@warning "-45"]
  ReactIntl.{id: [%e idExp], defaultMessage: [%e messageExp]};
};

let makeValuesType = (~loc, fields: list((string, core_type))): core_type => {
  let objectFields =
    fields
    |> List.map(((label, coreType)) =>
         {
           pof_desc: Otag({txt: label, loc}, coreType),
           pof_loc: loc,
           pof_attributes: [],
         }
       );

  {
    ptyp_desc: Ptyp_object(objectFields, Closed),
    ptyp_loc: loc,
    ptyp_loc_stack: [loc],
    ptyp_attributes: [],
  };
};

let makeString = (~payload, ~loc) => {
  let recordExp = makeIntlRecord(~payload, ~loc);
  let (message, _messageExp, _description) = parsePayload(~loc, payload);
  let pluralVariables =
    message
    |> Regexp.findAll(~regexp=Regexp.plural)
    |> List.map(label => (label, [%type: int]));
  let cleanedMessage =
    pluralVariables == []
      ? message : message |> Regexp.remove(~regexp=Regexp.plural);
  let simpleVariables =
    cleanedMessage
    |> Regexp.findAll(~regexp=Regexp.variable)
    |> List.map(label => (label, [%type: string]));
  let variables = simpleVariables @ pluralVariables;

  switch (variables) {
  | [] =>
    %expr
    ReactIntlPpxAdaptor.Message.to_s([%e recordExp])
  | variables =>
    let valuesType = variables |> makeValuesType(~loc);
    %expr
    (
      (values: Js.t([%t valuesType])) => (
        ReactIntlPpxAdaptor.Message.format_to_s([%e recordExp], values): string
      )
    );
  };
};

let makeReactElement = (~payload, ~loc) => {
  let recordExp = makeIntlRecord(~payload, ~loc);
  let (message, _messageExp, _description) = parsePayload(~loc, payload);
  let pluralVariables =
    message
    |> Regexp.findAll(~regexp=Regexp.plural)
    |> List.map(label => (label, [%type: int]));
  let cleanedMessage = message |> Regexp.remove(~regexp=Regexp.plural);
  let simpleVariables =
    cleanedMessage
    |> Regexp.findAll(~regexp=Regexp.variable)
    |> List.map(label => (label, [%type: React.element]));
  let richTextVariables =
    cleanedMessage
    |> Regexp.findAll(~regexp=Regexp.richText)
    |> List.map(label => (label, [%type: string => React.element]));
  let variables = simpleVariables @ pluralVariables @ richTextVariables;

  switch (variables) {
  | [] =>
    %expr
    React.string(ReactIntlPpxAdaptor.Message.to_s([%e recordExp]))
  | variables =>
    let valuesType = variables |> makeValuesType(~loc);
    %expr
    (
      (values: Js.t([%t valuesType])) =>
        React.string(
          ReactIntlPpxAdaptor.Message.format_to_s([%e recordExp], values)
        )
    );
  };
};
