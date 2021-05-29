open Ppxlib;

let makeId = (~description="", message) =>
  message ++ "|" ++ description |> Digest.string |> Digest.to_hex;

let makeIntlRecord = (~message, ~description=?, ~messageExp, ~loc, ()) => {
  let id = makeId(~description?, message);
  let idExp = Ast_helper.Exp.constant(Pconst_string(id, loc, None));
  %expr
  [@warning "-45"]
  ReactIntl.{id: [%e idExp], defaultMessage: [%e messageExp]};
};

let makeStringResolver = (~message, ~description=?, ~messageExp, ~loc, ()) => {
  let recordExp =
    makeIntlRecord(~message, ~description?, ~messageExp, ~loc, ());
  %expr
  ReactIntlPpxAdaptor.Message.to_s([%e recordExp]);
};

let makeReactElementResolver =
    (~description=?, ~message, ~messageExp, ~loc, ()) => {
  let stringResolverExp =
    makeStringResolver(~message, ~description?, ~messageExp, ~loc, ());
  %expr
  React.string([%e stringResolverExp]);
};

class mapper = {
  as _;
  inherit class Ast_traverse.map as super;
  pub! expression = e =>
    switch (e) {
    // Match [%intl "message"]
    | [%expr
        [%intl
          [%e?
            {pexp_desc: Pexp_constant(Pconst_string(message, _, _)), _} as messageExp
          ]
        ]
      ] =>
      makeIntlRecord(~message, ~messageExp, ~loc=e.pexp_loc, ())

    // Match [%intl {msg: "message", desc: "description"}]
    // and [%intl {desc: "description", msg: "message"}]
    | [%expr
        [%intl
          [%e?
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
            }
          ]
        ]
      ] =>
      makeIntlRecord(~message, ~description, ~messageExp, ~loc=e.pexp_loc, ())

    // Match [%intl.s "message"]
    | [%expr
        [%intl.s
          [%e?
            {pexp_desc: Pexp_constant(Pconst_string(message, _, _)), _} as messageExp
          ]
        ]
      ] =>
      makeStringResolver(~message, ~messageExp, ~loc=e.pexp_loc, ())

    // Match [%intl.s {msg: "message", desc: "description}]
    // and [%intl.s {desc: "description", msg: "message"}]
    | [%expr
        [%intl.s
          [%e?
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
            }
          ]
        ]
      ] =>
      makeStringResolver(
        ~message,
        ~description,
        ~messageExp,
        ~loc=e.pexp_loc,
        (),
      )

    // Match [%intl.el "message"]
    | [%expr
        [%intl.el
          [%e?
            {pexp_desc: Pexp_constant(Pconst_string(message, _, _)), _} as messageExp
          ]
        ]
      ] =>
      makeReactElementResolver(~message, ~messageExp, ~loc=e.pexp_loc, ())

    // Match [%intl.el {msg: "message", desc: "description"}]
    // And [%intl.el {desc: "description", msg: "message"}]
    | [%expr
        [%intl.el
          [%e?
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
            }
          ]
        ]
      ] =>
      makeReactElementResolver(
        ~message,
        ~description,
        ~messageExp,
        ~loc=e.pexp_loc,
        (),
      )
    | {
        pexp_desc:
          Pexp_extension(({txt: "intl" | "intl.s" | "intl.el", loc}, _)),
        _,
      } =>
      Location.raise_errorf(
        ~loc,
        "react-intl-ppx expects the extension payload to be a constant string or a record ({msg: string, desc: string}), it does not work with any other expression types.",
      )
    | _ => super#expression(e)
    };
};

let structure_mapper = s => (new mapper)#structure(s);
let () =
  Ppxlib.Driver.register_transformation(
    ~impl=structure_mapper,
    "bs-react-intl-ppx",
  );
