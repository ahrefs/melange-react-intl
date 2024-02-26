open Ppxlib;

let extractMessage = (~loc, expression) =>
  switch (expression.pexp_desc) {
  // Match "message"
  | Pexp_constant(Pconst_string(message, _, _)) => (
      message,
      expression,
      None,
    )

  // Match {msg: "message", desc: "description"} and {desc: "description", msg: "message"}
  | Pexp_record(
      [
        (
          {txt: Lident("msg"), _},
          {pexp_desc: Pexp_constant(Pconst_string(message, _, _)), _} as messageExp,
        ),
        (
          {txt: Lident("desc"), _},
          {pexp_desc: Pexp_constant(Pconst_string(description, _, _)), _},
        ),
      ] |
      [
        (
          {txt: Lident("desc"), _},
          {pexp_desc: Pexp_constant(Pconst_string(description, _, _)), _},
        ),
        (
          {txt: Lident("msg"), _},
          {pexp_desc: Pexp_constant(Pconst_string(message, _, _)), _} as messageExp,
        ),
      ],
      None,
    ) => (
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

module IntlRecord = {
  let extractor = Ast_pattern.(single_expr_payload(__));

  let expression_handler = (~ctxt as _, expression) => {
    let loc = expression.pexp_loc;
    let (messsage, messageExpr, description) =
      extractMessage(~loc, expression);
    Resolver.makeIntlRecord(~loc, messsage, messageExpr, description);
  };

  let context_free = label =>
    Context_free.Rule.extension(
      Extension.V3.declare(
        label,
        Extension.Context.expression,
        extractor,
        expression_handler,
      ),
    );
};

module IntlString = {
  let extractor = Ast_pattern.(single_expr_payload(__));

  let expression_handler = (~ctxt as _, expression) => {
    let loc = expression.pexp_loc;
    let (messsage, messageExpr, description) =
      extractMessage(~loc, expression);
    Resolver.makeString(~loc, messsage, messageExpr, description);
  };

  let context_free = label =>
    Context_free.Rule.extension(
      Extension.V3.declare(
        label,
        Extension.Context.expression,
        extractor,
        expression_handler,
      ),
    );
};

module IntlReact = {
  let extractor = Ast_pattern.(single_expr_payload(__));

  let expression_handler = (~ctxt as _, expression) => {
    let loc = expression.pexp_loc;
    let (messsage, messageExpr, description) =
      extractMessage(~loc, expression);
    Resolver.makeReactElement(~loc, messsage, messageExpr, description);
  };

  let context_free = label =>
    Context_free.Rule.extension(
      Extension.V3.declare(
        label,
        Extension.Context.expression,
        extractor,
        expression_handler,
      ),
    );
};

let () = {
  Driver.V2.register_transformation(
    ~rules=[
      IntlRecord.context_free("intl"),
      IntlRecord.context_free("intl_draft"),
      IntlString.context_free("intl.s"),
      IntlString.context_free("intl_draft.s"),
      IntlReact.context_free("intl.el"),
      IntlReact.context_free("intl_draft.el"),
    ],
    "melange-react-intl.ppx",
  );
};
