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
