Basic case

  $ cat > input.re <<EOF
  > [%intl.el {js|{maxLength} characters maximum|js}] @@ {"maxLength": length->NumFormat.Int.format->RR.s}
  > EOF

  $ ./ppx.sh --output re input.re
  $TESTCASE_ROOT
  (
    (values: {. "maxLength": React.element}) =>
      React.string(
        ReactIntlPpxAdaptor.Message.format_to_s(
          ~list_of_values=[("maxLength", `Element(values#maxLength))],
          [@warning "-45"]
          ReactIntl.{
            id: "52d92c9e4920f7e245381fad58360708",
            defaultMessage: {js|{maxLength} characters maximum|js},
          },
          values,
        ),
      )
  ) @@
  {"maxLength": length->NumFormat.Int.format->RR.s};

With plural

  $ cat > input.re <<EOF
  > let t = [%intl.s
  >   {
  >     msg: {js|Failed to remove {keywordsCount, plural, zero {keywords} one {keyword} few {keywords} other {keywords}} from "{listName}".|js},
  >     desc: {js|Failed to remove keywords from "My list".|js},
  >   }
  > ] @@
  > {"keywordsCount": keywords->List.length, "listName": name}
  > EOF

  $ ./ppx.sh --output re input.re
  $TESTCASE_ROOT
  let t =
    (
      (
        values: {
          .
          "listName": string,
          "keywordsCount": int,
        },
      ) => (
        ReactIntlPpxAdaptor.Message.format_to_s(
          [@warning "-45"]
          ReactIntl.{
            id: "0df6b0b4831df770629c8980275ccf00",
            defaultMessage: {js|Failed to remove {keywordsCount, plural, zero {keywords} one {keyword} few {keywords} other {keywords}} from "{listName}".|js},
          },
          values,
        ): string
      )
    ) @@
    {"keywordsCount": keywords->List.length, "listName": name};
