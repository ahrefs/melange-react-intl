open Ppxlib;
module Builder = Ppxlib.Ast_builder.Default;

let makeId = (~description="", message) =>
  message ++ "|" ++ description |> Digest.string |> Digest.to_hex;

let tryUnescape = s =>
  try(Scanf.unescaped(s)) {
  | Scanf.Scan_failure(_err) => s
  };

let warning_45 = (~loc) =>
  Builder.attribute(
    ~loc,
    ~name={Location.loc, txt: "warning"},
    ~payload=
      PStr([Builder.pstr_eval(~loc, Builder.estring(~loc, "-45"), [])]),
  );

let makeIntlRecord = (~loc, message, messageExp, description) => {
  let id = message |> tryUnescape |> makeId(~description?);
  let idExp = Ast_helper.Exp.constant(Pconst_string(id, loc, None));
  let expr = [%expr
    ReactIntl.{id: [%e idExp], defaultMessage: [%e messageExp]}
  ];
  {...expr, pexp_attributes: [warning_45(~loc)]};
};

let makeValuesAsList = (~loc, variables) => {
  let list_of_expr =
    variables
    |> List.map(((label, type_)) => {
         let key = Ast_helper.Exp.constant(Pconst_string(label, loc, None));
         /* Here we read the field from the Js.t `values` with `##` (values##variable_name) */
         let access_values_object =
           Ast_helper.Exp.apply(
             Ast_helper.Exp.ident({txt: Lident("##"), loc}),
             [
               (Nolabel, Ast_helper.Exp.ident({txt: Lident("values"), loc})),
               (Nolabel, Ast_helper.Exp.ident({txt: Lident(label), loc})),
             ],
           );
         let value =
           switch (type_.ptyp_desc) {
           | Ptyp_constr({txt: Lident("string")}, []) =>
             [%expr `String([%e access_values_object])]
           | Ptyp_constr({txt: Lident("int")}, []) =>
             [%expr `Number([%e access_values_object])]
           | Ptyp_constr({txt: Ldot(Lident("React"), "element")}, []) =>
             [%expr `Element([%e access_values_object])]
           | Ptyp_arrow(_, core_type_arg, core_type_return) =>
             switch (core_type_arg.ptyp_desc, core_type_return.ptyp_desc) {
             | (
                 Ptyp_constr({txt: Lident("string")}, []),
                 Ptyp_constr({txt: Ldot(Lident("React"), "element")}, []),
               ) =>
               [%expr `Component([%e access_values_object])]
             | _ =>
               [%expr
                [%ocaml.error
                  "melange-react-intl: Unsupported rich text variable type, only string => React.element is supported"
                ]]
             }
           | _ =>
             [%expr
              [%ocaml.error
                "melange-react-intl: Unsupported variable type, only `string`, `int`, `React.element` or `string => React.element` are supported"
              ]]
           };
         [%expr ([%e key], [%e value])];
       });

  [%expr [[%e Ast_helper.Exp.tuple(list_of_expr)]]];
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
  | [] => [%expr ReactIntlPpxAdaptor.Message.to_s([%e recordExp])]
  | variables =>
    let valuesType = variables |> makeValuesType(~loc);
    let listAsValues = makeValuesAsList(~loc, variables);
    [%expr
     (
       (values: Js.t([%t valuesType])) => (
         ReactIntlPpxAdaptor.Message.format_to_s(
           ~list_of_values=[%e listAsValues],
           [%e recordExp],
           values,
         ): string
       )
     )];
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
    [%expr React.string(ReactIntlPpxAdaptor.Message.to_s([%e recordExp]))]
  | variables =>
    let valuesType = variables |> makeValuesType(~loc);
    let listAsValues = makeValuesAsList(~loc, variables);
    [%expr
     (
       (values: Js.t([%t valuesType])) =>
         React.string(
           ReactIntlPpxAdaptor.Message.format_to_s(
             ~list_of_values=[%e listAsValues],
             [%e recordExp],
             values,
           ),
         )
     )];
  };
};
