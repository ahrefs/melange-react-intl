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

class mapper = {
  as _;
  inherit class Ast_traverse.map as super;
  pub! expression = e =>
    switch (e) {
    | {pexp_desc: Pexp_extension(({txt: "intl" | "intl_draft"}, payload))} =>
      let (message, messageExp, description) =
        parsePayload(~loc=e.pexp_loc, payload);
      Resolver.makeIntlRecord(
        message,
        messageExp,
        description,
        ~loc=e.pexp_loc,
      );

    | {
        pexp_desc:
          Pexp_extension(({txt: "intl.s" | "intl_draft.s"}, payload)),
      } =>
      let (message, messageExp, description) =
        parsePayload(~loc=e.pexp_loc, payload);
      Resolver.makeString(~loc=e.pexp_loc, message, messageExp, description);

    | {
        pexp_desc:
          Pexp_extension(({txt: "intl.el" | "intl_draft.el"}, payload)),
      } =>
      let (message, messageExp, description) =
        parsePayload(~loc=e.pexp_loc, payload);
      Resolver.makeReactElement(
        ~loc=e.pexp_loc,
        message,
        messageExp,
        description,
      );

    | _ => super#expression(e)
    };
};

module IntlRecord = {
  let extractor = Ast_pattern.(single_expr_payload(__));

  let expression_handler = (~ctxt as _, _) => {
    let loc = Location.none;
    [%expr "lola"];
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

  let expression_handler = (~ctxt as _, _) => {
    let loc = Location.none;
    [%expr "lola"];
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

  let expression_handler = (~ctxt as _, _) => {
    let loc = Location.none;
    [%expr "lola"];
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

let structure_mapper = s => (new mapper)#structure(s);

let () = {
  Driver.register_transformation(
    ~impl=structure_mapper,
    "melange-react-intl.ppx-old",
  );
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
