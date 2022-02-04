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

let makeStringResolver = (~payload, ~loc) => {
  let recordExp = makeIntlRecord(~payload, ~loc);
  %expr
  ReactIntlPpxAdaptor.Message.to_s([%e recordExp]);
};

let makeReactElementResolver = (~payload, ~loc) => {
  let stringResolverExp = makeStringResolver(~payload, ~loc);
  %expr
  React.string([%e stringResolverExp]);
};

class mapper = {
  as _;
  inherit class Ast_traverse.map as super;
  pub! expression = e =>
    switch (e) {
    | {
        pexp_desc:
          Pexp_extension(({txt: "intl" | "intl_stub", loc}, payload)),
      } =>
      makeIntlRecord(~payload, ~loc=e.pexp_loc)

    | {
        pexp_desc:
          Pexp_extension(({txt: "intl.s" | "intl_stub.s", loc}, payload)),
      } =>
      makeStringResolver(~payload, ~loc=e.pexp_loc)

    | {
        pexp_desc:
          Pexp_extension(({txt: "intl.el" | "intl_stub.el", loc}, payload)),
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
