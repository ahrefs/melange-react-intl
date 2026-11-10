  $ ../ppx.sh --output re input.re
  let message: ReactIntl.message =
    [@warning "-45"]
    ReactIntl.{
      id: "24ac19a52c2b0192688fa8294d28dbae",
      defaultMessage: "i am message",
      maxLength: None,
    };
  let descriptedMessage: ReactIntl.message =
    [@warning "-45"]
    ReactIntl.{
      id: "6142e0d7a3dbfa8e6cbb4cfd716515fa",
      defaultMessage: "i am message",
      maxLength: None,
    };
  let message2: string =
    ReactIntlPpxAdaptor.Message.to_s(
      [@warning "-45"]
      ReactIntl.{
        id: "900317ffb9c870f401a04bef9fd4b5b3",
        defaultMessage: "blabla",
        maxLength: None,
      },
    );
  let descriptedmessage2: string =
    ReactIntlPpxAdaptor.Message.to_s(
      [@warning "-45"]
      ReactIntl.{
        id: "6142e0d7a3dbfa8e6cbb4cfd716515fa",
        defaultMessage: "i am message",
        maxLength: None,
      },
    );
  let undescriptedmessage: string =
    ReactIntlPpxAdaptor.Message.to_s(
      [@warning "-45"]
      ReactIntl.{
        id: "24ac19a52c2b0192688fa8294d28dbae",
        defaultMessage: "i am message",
        maxLength: None,
      },
    );
  let element3: React.element =
    React.string(
      ReactIntlPpxAdaptor.Message.to_s(
        [@warning "-45"]
        ReactIntl.{
          id: "900317ffb9c870f401a04bef9fd4b5b3",
          defaultMessage: "blabla",
          maxLength: None,
        },
      ),
    );
  let descriptedElement3: React.element =
    React.string(
      ReactIntlPpxAdaptor.Message.to_s(
        [@warning "-45"]
        ReactIntl.{
          id: "6142e0d7a3dbfa8e6cbb4cfd716515fa",
          defaultMessage: "i am message",
          maxLength: None,
        },
      ),
    );
  let stringWithVariable: {. "variable": string} => string =
    (values: {. "variable": string}) => (
      ReactIntlPpxAdaptor.Message.format_to_s(
        ~list_of_values=[("variable", `String(values##variable))],
        [@warning "-45"]
        ReactIntl.{
          id: "4271c9d35b2eac4fca6faf3e2eeb6019",
          defaultMessage: "I am string with {variable}",
          maxLength: None,
        },
        values,
      ): string
    );
  let stringWithPluralForm: {. "itemsCount": int} => string =
    (values: {. "itemsCount": int}) => (
      ReactIntlPpxAdaptor.Message.format_to_s(
        ~list_of_values=[("itemsCount", `Number(values##itemsCount))],
        [@warning "-45"]
        ReactIntl.{
          id: "a2926a901acebbdc606104ceae81dc82",
          defaultMessage: "{itemsCount, plural, zero {item} one {item} few {items} many {items} other {items}}",
          maxLength: None,
        },
        values,
      ): string
    );
  let elementWithVariable: {. "variable": React.element} => React.element =
    (values: {. "variable": React.element}) =>
      ReactIntlPpxAdaptor.Message.format_to_el(
        ~list_of_values=[("variable", `Element(values##variable))],
        [@warning "-45"]
        ReactIntl.{
          id: "ac400e3c977990cd86a6981ad7eef8cd",
          defaultMessage: "I am react element with {variable}",
          maxLength: None,
        },
        values,
      );
  let elementWithPluralForm: {. "itemsCount": int} => React.element =
    (values: {. "itemsCount": int}) =>
      ReactIntlPpxAdaptor.Message.format_to_el(
        ~list_of_values=[("itemsCount", `Number(values##itemsCount))],
        [@warning "-45"]
        ReactIntl.{
          id: "a2926a901acebbdc606104ceae81dc82",
          defaultMessage: "{itemsCount, plural, zero {item} one {item} few {items} many {items} other {items}}",
          maxLength: None,
        },
        values,
      );
  let stringWithPluralFormNoZero: {. "itemsCount": int} => string =
    (values: {. "itemsCount": int}) => (
      ReactIntlPpxAdaptor.Message.format_to_s(
        ~list_of_values=[("itemsCount", `Number(values##itemsCount))],
        [@warning "-45"]
        ReactIntl.{
          id: "4bd22beea7e3314a3932db7f44bf5d09",
          defaultMessage: "{itemsCount, plural, one {item} few {items} many {items} other {items}}",
          maxLength: None,
        },
        values,
      ): string
    );
  let stringWithPluralFormNoFew: {. "itemsCount": int} => string =
    (values: {. "itemsCount": int}) => (
      ReactIntlPpxAdaptor.Message.format_to_s(
        ~list_of_values=[("itemsCount", `Number(values##itemsCount))],
        [@warning "-45"]
        ReactIntl.{
          id: "16803d1a875cd1508f82a14149b1a1e0",
          defaultMessage: "{itemsCount, plural, zero {item} one {item} many {items} other {items}}",
          maxLength: None,
        },
        values,
      ): string
    );
  let stringWithPluralFormNoZeroNoFew: {. "itemsCount": int} => string =
    (values: {. "itemsCount": int}) => (
      ReactIntlPpxAdaptor.Message.format_to_s(
        ~list_of_values=[("itemsCount", `Number(values##itemsCount))],
        [@warning "-45"]
        ReactIntl.{
          id: "885a4d7a970a7515c3e340cdaccfc03b",
          defaultMessage: "{itemsCount, plural, one {item} other {items}}",
          maxLength: None,
        },
        values,
      ): string
    );
  let stringWithPluralFormNoOne: {. "itemsCount": int} => string =
    (values: {. "itemsCount": int}) => (
      ReactIntlPpxAdaptor.Message.format_to_s(
        ~list_of_values=[("itemsCount", `Number(values##itemsCount))],
        [@warning "-45"]
        ReactIntl.{
          id: "45bf442cb090ff6a13b064b17e14300e",
          defaultMessage: "{itemsCount, plural, zero {item} few {items} many {items} other {items}}",
          maxLength: None,
        },
        values,
      ): string
    );
  let stringWithPluralFormOnlyOther: {. "itemsCount": int} => string =
    (values: {. "itemsCount": int}) => (
      ReactIntlPpxAdaptor.Message.format_to_s(
        ~list_of_values=[("itemsCount", `Number(values##itemsCount))],
        [@warning "-45"]
        ReactIntl.{
          id: "081ae6d38f86613b243dd3e97503454c",
          defaultMessage: "{itemsCount, plural, other {items}}",
          maxLength: None,
        },
        values,
      ): string
    );
  let stringWithPluralBranchVariable:
    {
      .
      "total": int,
      "totalStr": string,
    } =>
    string =
    (
      values: {
        .
        "totalStr": string,
        "total": int,
      },
    ) => (
      ReactIntlPpxAdaptor.Message.format_to_s(
        ~list_of_values=[("totalStr", `String(values##totalStr)), ("total", `Number(values##total))],
        [@warning "-45"]
        ReactIntl.{
          id: "6a72218649361384d74c19c44c5faea7",
          defaultMessage: "{total, plural, one {Select {totalStr} prompt} other {Select all {totalStr} prompts}}",
          maxLength: None,
        },
        values,
      ): string
    );
  let stringWithPluralBranchPunctuation:
    {
      .
      "count": int,
      "countStr": string,
      "name": string,
    } =>
    string =
    (
      values: {
        .
        "countStr": string,
        "name": string,
        "count": int,
      },
    ) => (
      ReactIntlPpxAdaptor.Message.format_to_s(
        ~list_of_values=[
          ("countStr", `String(values##countStr)),
          ("name", `String(values##name)),
          ("count", `Number(values##count)),
        ],
        [@warning "-45"]
        ReactIntl.{
          id: "8ad381f34ef5e7bda864780b8d9db0c9",
          defaultMessage: "{name}: {count, plural, one {You've selected {countStr} item.} other {You've selected {countStr} items, {name}.}}",
          maxLength: None,
        },
        values,
      ): string
    );
  let stringWithExactPluralSelector:
    {
      .
      "count": int,
      "folder": string,
    } =>
    string =
    (
      values: {
        .
        "folder": string,
        "count": int,
      },
    ) => (
      ReactIntlPpxAdaptor.Message.format_to_s(
        ~list_of_values=[("folder", `String(values##folder)), ("count", `Number(values##count))],
        [@warning "-45"]
        ReactIntl.{
          id: "259df0fb35471979ad5c89945ebb9d30",
          defaultMessage: "{count, plural, =0 {No items} other {{count} items in {folder}}}",
          maxLength: None,
        },
        values,
      ): string
    );
  let elementWithPluralBranchVariable:
    {
      .
      "total": int,
      "totalStr": React.element,
    } =>
    React.element =
    (
      values: {
        .
        "totalStr": React.element,
        "total": int,
      },
    ) =>
      ReactIntlPpxAdaptor.Message.format_to_el(
        ~list_of_values=[("totalStr", `Element(values##totalStr)), ("total", `Number(values##total))],
        [@warning "-45"]
        ReactIntl.{
          id: "6a72218649361384d74c19c44c5faea7",
          defaultMessage: "{total, plural, one {Select {totalStr} prompt} other {Select all {totalStr} prompts}}",
          maxLength: None,
        },
        values,
      );
  let elementWithRichText: {. "a": string => React.element} => React.element =
    (values: {. "a": string => React.element}) =>
      ReactIntlPpxAdaptor.Message.format_to_el(
        ~list_of_values=[("a", `Component(values##a))],
        [@warning "-45"]
        ReactIntl.{
          id: "c1d9f720d6a89b19f574a7de2bf55f62",
          defaultMessage: "Some text with <a>link text</a>",
          maxLength: None,
        },
        values,
      );
  let cellText = (~powerUsersCount) =>
    (
      (
        values: {
          .
          "powerUsersCountString": React.element,
          "powerUsersCount": int,
        },
      ) =>
        ReactIntlPpxAdaptor.Message.format_to_el(
          ~list_of_values=[
            ("powerUsersCountString", `Element(values##powerUsersCountString)),
            ("powerUsersCount", `Number(values##powerUsersCount)),
          ],
          [@warning "-45"]
          ReactIntl.{
            id: "f64fa55a351a8fe989d4fe05f15ec260",
            defaultMessage: {js|{powerUsersCountString} {powerUsersCount, plural, zero {Power users} one {Power user} few {Power users} other {Power users}}|js},
            maxLength: None,
          },
          values,
        )
    ) @@
    {
      "powerUsersCountString": powerUsersCount->RR.int,
      "powerUsersCount": powerUsersCount,
    };
  let elementWithNestedTags =
      (
        values: {
          .
          "a": string => React.element,
          "li": string => React.element,
          "p": string => React.element,
          "ul": string => React.element,
        },
      ) =>
    ReactIntlPpxAdaptor.Message.format_to_el(
      ~list_of_values=[
        ("a", `Component(values##a)),
        ("li", `Component(values##li)),
        ("p", `Component(values##p)),
        ("ul", `Component(values##ul)),
      ],
      [@warning "-45"]
      ReactIntl.{
        id: "33088cac6fe90955923da0de98805ab6",
        defaultMessage: {js|
    <ul>
      <li>one</li>
      <li>two</li>
    </ul>
    <opentag>
    <p>
      <a href="https://ocaml.com">link text</a>.
    </p>
  |js},
        maxLength: None,
      },
      values,
    );
  let messageWithMaxLength: ReactIntl.message =
    [@warning "-45"]
    ReactIntl.{
      id: "f8dac259171ff3728f7a3317e00e265c",
      defaultMessage: "i am message with maxLength",
      maxLength: Some(80),
    };
  $ ../ppx.sh --output re inputExceeding.re
  File "output.ml", line 4, characters 13-52:
  4 |        msg = "i am message with maxLength exceeding";
                   ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
  Error: Message length of "i am message with maxLength exceeding" (37) exceeds maxLength of 20
  [1]
