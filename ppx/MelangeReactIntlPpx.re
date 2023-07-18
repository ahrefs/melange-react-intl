open Ppxlib;

class mapper = {
  as _;
  inherit class Ast_traverse.map as super;
  pub! expression = e =>
    switch (e) {
    | {pexp_desc: Pexp_extension(({txt: "intl" | "intl_draft"}, payload))} =>
      Resolver.makeIntlRecord(~payload, ~loc=e.pexp_loc)

    | {
        pexp_desc:
          Pexp_extension(({txt: "intl.s" | "intl_draft.s"}, payload)),
      } =>
      Resolver.makeString(~payload, ~loc=e.pexp_loc)

    | {
        pexp_desc:
          Pexp_extension(({txt: "intl.el" | "intl_draft.el"}, payload)),
      } =>
      Resolver.makeReactElement(~payload, ~loc=e.pexp_loc)

    | _ => super#expression(e)
    };
};

let structure_mapper = s => (new mapper)#structure(s);

let () =
  Ppxlib.Driver.register_transformation(
    ~impl=structure_mapper,
    "melange-react-intl.ppx",
  );
