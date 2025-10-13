open Ppxlib;

/* Given an expression, extract the right shape from a string or a record. This could be refactored into Ast_pattern. */
let extractMessage = (~loc, expression) =>
  switch (expression.pexp_desc) {
  // Match a single string, which represents the "message"
  | Pexp_constant(Pconst_string(message, _, _)) => (message, expression, None)

  // Match a record and extract fields by name, regardless of order
  | Pexp_record(fields, None) =>
    let msgField = ref(None);
    let descField = ref(None);

    // Iterate through fields and extract the ones we care about
    List.iter(
      ((fieldName, fieldValue)) => {
        switch (fieldName.txt) {
        | Lident("msg") =>
          switch (fieldValue.pexp_desc) {
          | Pexp_constant(Pconst_string(message, _, _)) => msgField := Some((message, fieldValue))
          | _ =>
            Location.raise_errorf(
              ~loc=fieldValue.pexp_loc,
              "react-intl-ppx expects 'msg' field to be a constant string",
            )
          }
        | Lident("desc") =>
          switch (fieldValue.pexp_desc) {
          | Pexp_constant(Pconst_string(description, _, _)) => descField := Some(description)
          | _ =>
            Location.raise_errorf(
              ~loc=fieldValue.pexp_loc,
              "react-intl-ppx expects 'desc' field to be a constant string",
            )
          }
        | _ => () // Ignore unknown fields for now
        }
      },
      fields,
    );

    // Validate that required fields are present
    switch (msgField^, descField^) {
    | (Some((message, messageExp)), desc) => (message, messageExp, desc)
    | (None, _) => Location.raise_errorf(~loc, "react-intl-ppx requires a 'msg' field in the record")
    };

  | _ =>
    Location.raise_errorf(
      ~loc,
      "react-intl-ppx expects the extension payload to be a constant string or a record ({msg: string, desc: string}), it does not work with any other expression types.",
    )
  };

let context_free_expression_tranform = (label, transform) => {
  /* This pattern matches any single expression under a PStr */
  let single_expression_payload = Ast_pattern.(single_expr_payload(__));

  Context_free.Rule.extension(
    Extension.V3.declare(label, Extension.Context.expression, single_expression_payload, (~ctxt as _, expression) => {
      transform(~loc=expression.pexp_loc, expression)
    }),
  );
};

let () = {
  Driver.V2.register_transformation(
    ~rules=[
      context_free_expression_tranform("intl", (~loc, expression) => {
        let (messsage, messageExpr, description) = extractMessage(~loc, expression);
        Resolver.makeIntlRecord(~loc, messsage, messageExpr, description);
      }),
      context_free_expression_tranform("intl.s", (~loc, expression) => {
        let (messsage, messageExpr, description) = extractMessage(~loc, expression);
        Resolver.makeString(~loc, messsage, messageExpr, description);
      }),
      context_free_expression_tranform("intl.el", (~loc, expression) => {
        let (messsage, messageExpr, description) = extractMessage(~loc, expression);
        Resolver.makeReactElement(~loc, messsage, messageExpr, description);
      }),
    ],
    "melange-react-intl.ppx",
  );
};
