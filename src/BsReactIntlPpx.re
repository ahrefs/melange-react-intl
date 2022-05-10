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
  let id = makeId(~description?, message);
  let idExp = Ast_helper.Exp.constant(Pconst_string(id, loc, None));
  %expr
  [@warning "-45"]
  ReactIntl.{id: [%e idExp], defaultMessage: [%e messageExp]};
};

type objectFields = list((string, core_type));

let makeValuesType = (~loc, fields: objectFields): core_type => {
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

module Regexp = {
  let variable = Re.Pcre.regexp("\\{(\\w+)}");
  let plural =
    Re.Pcre.regexp(
      "\\{(\\w+), plural, zero \\{[^\\}]+\\} one \\{[^\\}]+\\} few \\{[^\\}]+\\}(?: many \\{[^\\}]+\\})? other \\{[A-Za-z ]+\\}\\}",
    );
  let richText = Re.Pcre.regexp("<(\\w+)>[^<]+</\\w+>");

  let findAll = (~regexp, s) =>
    Re.all(regexp, s)
    /* every match implicitly has a group 0 that covers the whole match, */
    /* and explicit groups are numbered from 1 */
    |> List.map(Re.Group.get(_, 1))
    |> List.sort_uniq(String.compare);

  let remove = (~regexp, s) =>
    Re.replace_string(~all=true, regexp, ~by="", s);
};

let makeStringResolver = (~payload, ~loc) => {
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

let makeReactElementResolver = (~payload, ~loc) => {
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
    ReactIntlPpxAdaptor.Message.to_s([%e recordExp])->React.string
  | variables =>
    let valuesType = variables |> makeValuesType(~loc);
    %expr
    (
      (values: Js.t([%t valuesType])) =>
        ReactIntlPpxAdaptor.Message.format_to_s([%e recordExp], values)
        ->React.string
    );
  };
};

class mapper = {
  as _;
  inherit class Ast_traverse.map as super;
  pub! expression = e =>
    switch (e) {
    | {pexp_desc: Pexp_extension(({txt: "intl" | "intl_draft"}, payload))} =>
      makeIntlRecord(~payload, ~loc=e.pexp_loc)

    | {
        pexp_desc:
          Pexp_extension(({txt: "intl.s" | "intl_draft.s"}, payload)),
      } =>
      makeStringResolver(~payload, ~loc=e.pexp_loc)

    | {
        pexp_desc:
          Pexp_extension(({txt: "intl.el" | "intl_draft.el"}, payload)),
      } =>
      makeReactElementResolver(~payload, ~loc=e.pexp_loc)

    | _ => super#expression(e)
    };
};

let structure_mapper = s => (new mapper)#structure(s);

let () =
  Ppxlib.Driver.register_transformation(
    ~impl=structure_mapper,
    "bs-react-intl-ppx",
  );
