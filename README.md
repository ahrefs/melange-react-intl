# melange-react-intl

The [react-intl](https://formatjs.io/docs/react-intl/) bindings for
[Melange](https://melange.re/) and a
[preprocessor](https://ocaml.org/docs/metaprogramming) that eases the creation
of translated messages.

## Installation

Install the [opam](https://opam.ocaml.org/) package manager, and then run:

```shell
opam pin add melange-react-intl.dev git+https://github.com/ahrefs/melange-react-intl.git#master
```

## Overview

The package consists of two parts:
- The bindings to the `react-intl` JavaScript library.
- The PPX, which allows you to create `ReactIntl.messages` records without
  specifying `id`, significantly reducing the amount of boilerplate code and
  simplifying the translation process.

### Examples

This ReasonML code:

```reason
let message: ReactIntl.message = [%intl "I am a message"];
```

will be compiled to JavaScript as:

```javascript
var message = {
  id: "168c9a2987fad481c5882847ac102aaf",
  defaultMessage: "I am a message"
};
```

The PPX also supports messages with a description:

```reason
let withDescription: ReactIntl.message = [%intl {msg: "blabla", desc: "I am a description"}];
```

## Usage with the bindings

```reason
[@react.component]
let make = () => {
  let intl = ReactIntl.useIntl();
  // Helper functions
  let l = message => intl->ReactIntl.Intl.formatMessage(message);

  <>
    <h1>[%intl "Some header"]->l->React.string</h1>
    <p>[%intl "Some body"]->l->React.string</p>
  </>
}
```

You can define your own `FormattedMessage` component, which accepts a
`ReactIntl.message` instead of `id` and `defaultMessage`:

```reason
/// FormattedMessage.re
[@react.component]
let make = (~item: ReactIntl.message, ~values: Js.t({..})) =>
  <ReactIntl.FormattedMessage id={item.id} defaultMessage={item.defaultMessage} values />;
```

## Draft phrases

If you want to make the
[extractor](https://github.com/cca-io/rescript-react-intl-extractor) ignore some
phrases, you can use `intl_draft`, `intl_draft.s`, or `intl_draft.el`
extensions. This is helpful if you don't want some draft phrases (likely to
change soon) to be sent to translators.

## Advanced usage

There is an option to generate a localized string straight from the PPX (without
explicit helper functions usage). This option could be helpful if you don't need
to change the page language without page reloading.

- Support for the `intl.s` annotation:

```reason
let message: string = [%intl.s "I am a message"]; // type string
```

Converts to ⬇️

```reason
let message: string = {id: "168c9a2987fad481c5882847ac102aaf", defaultMessage: "I am a message"}->ReactIntPpxAdaptor.Message.to_s;
```

- Support for the `intl.el` annotation:

```reason
let element: React.element = [%intl.el "I am a message"];
```

Converts to ⬇️

```reason
let element: React.element = {id: "168c9a2987fad481c5882847ac102aaf", defaultMessage: "I am a message"}->ReactIntPpxAdaptor.Message.to_s->React.string;
```

- Support for variables, plural forms, and [rich text
  formatting](https://formatjs.io/docs/react-intl/components/#rich-text-formatting)
  in payload. In this case, the PPX will return a function instead of
  `ReactIntl.message`:

```reason
let element: React.element = [%intl.el "I am a message with {variable}"];
```

Converts to ⬇️

```reason
let element: React.element =
  (values: {. "variable": React.element}) => {id: "168c9a2987fad481c5kgmcntg5k3dsd5", defaultMessage: "I am a message with {variable}"}->ReactIntPpxAdaptor.Message.format_to_s(_, values)->React.string;
```

To use these features, you have to define `ReactIntlPpxAdaptor` in your app (see
the `test` folder for details).
