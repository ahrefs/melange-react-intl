# melange-react-intl-ppx

These are the [Melange](https://melange.re/) bindings for [react-intl](https://github.com/formatjs/formatjs/tree/main/packages/react-intl).

## Installation
Install [opam](https://opam.ocaml.org/) package manager.
Then:

```shell
opam pin add melange-react-intl-ppx.dev git+https://github.com/ahrefs/bs-react-intl-ppx.git#eb36bab7565552fffcc59917913220f52c643db8
```

Then `react-intl` npm package should also be installed:

```json
"react-intl": "^6.4.4",
```

## Overview

This ppx allows you to create `ReactIntl.messages` records without specifying `id`, which significantly reduces the amount of boilerplate code and simplifies the translation process.

Examples:

This code
```reason
let message: ReactIntl.message = [%intl "i am message"]
```

will be compiled to the (javascript):
```javascript
var message = {
  id: "168c9a2987fad481c5882847ac102aaf",
  defaultMessage: "i am message"
};
```

Ppx also supports descripted messages:

```reason
let descriptedMessage: ReactIntl.message = [%intl {msg: "blabla", desc: "i am description"}]
```

## Usage with [rescript-react-intl](https://github.com/cca-io/rescript-react-intl)

```reason
// Some component
[@react.component]
let make = () => {
  let intl = ReactIntl.useIntl();
  // helper functions
  let l = message => intl->ReactIntl.Intl.formatMessage(message);

  <>
    <h1>[%intl "Some header"]->l->React.string</h1>
    <p>[%intl "Some body"]->l->React.string</p>
  </>
}
```


You can define your own `FormattedMessage` component, which accepts `ReactIntl.message` instead of `id` and `defaultMessage`.

```reason
/// FormattedMessage.re
[@react.component]
let make = (~item: ReactIntl.message, ~values: Js.t({..})) =>
  <ReactIntl.FormattedMessage id={item.id} defaultMessage={item.defaultMessage} values />;
```

## Draft phrases

If you want to make [extactor](https://github.com/cca-io/rescript-react-intl-extractor) ignore some phrases, you can use `intl_draft` / `intl_draft.s`/ `intl_draft.el` extensions.
It is helpful if you don't want some draft phrases (likely to change soon) to be sent to translators.

## Advanced usage

There is an option to generate localized string straight from the ppx (without explicit helper functions usage).
This option could be helpful for you if you don't need to change page language without page reloading.

- Ppx supports `intl.s` annotation:
```reason
let message: string = [%intl.s "i am message"] // type string
```
converts to ⬇️
```reason
let message: string = {id: "168c9a2987fad481c5882847ac102aaf", defaultMessage: "i am message"}->ReactIntPpxAdaptor.Message.to_s
```
- Ppx supports `intl.el` annotation:
```reason
let element: React.element = [%intl.el "i am message"]
```
converts to ⬇️
```reason
let element: React.element = {id: "168c9a2987fad481c5882847ac102aaf", defaultMessage: "i am message"}->ReactIntPpxAdaptor.Message.to_s->React.string
```

- Ppx supports variables, plural forms and [rich text formatting](https://formatjs.io/docs/react-intl/components/#rich-text-formatting) in payload. In this case ppx will return function instead of ReactIntl.message
```reason
let element: React.element = [%intl.el "i am message with {variable}"]
```
converts to ⬇️
```reason
let element: React.element =
  (values: {. "variable": React.element}) => {id: "168c9a2987fad481c5kgmcntg5k3dsd5", defaultMessage: "i am message with {variable}"}->ReactIntPpxAdaptor.Message.format_to_s(_, values)->React.string
```

To use those features you have to define `ReactIntlPpxAdaptor` in your app (see test folder for details).
