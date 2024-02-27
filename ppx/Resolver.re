open Ppxlib;

let makeId = (~description="", message) =>
  message ++ "|" ++ description |> Digest.string |> Digest.to_hex;

let tryUnescape = s =>
  try(Scanf.unescaped(s)) {
  | Scanf.Scan_failure(_err) => s
  };

let makeIntlRecord = (~loc, message, messageExp, description) => {
  let id = message |> tryUnescape |> makeId(~description?);
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

let makeString = (~loc, message, messageExp, description) => {
  let recordExp = makeIntlRecord(~loc, message, messageExp, description);
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

let makeReactElement = (~loc, message, messageExp, description) => {
  let recordExp = makeIntlRecord(~loc, message, messageExp, description);
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