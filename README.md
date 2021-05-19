# bs-react-intl-ppx

## Installation
Get the package:

```shell
# yarn
yarn add --dev @ahrefs/bs-react-intl-ppx
# or npm
npm install --save-dev @ahrefs/bs-react-intl-ppx
```

Then add it to `bsconfig.json`:

```json
"ppx-flags": ["@ahrefs/bs-react-intl-ppx/ppx"],
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
    <h1>[%intl "Some header"]->l->React.string</h2>
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


## Advanced usage

There is an option to generate localized string straight from the ppx (without explicit helper functions usage).
This option could be helpful for you if you don't need to change page language without page reloading.

- Ppx supports `intl.s` annotation:
```reason
let message: string = [%intl.s "i am message"] // type string
```
Which will be converted to the:

```reason
let message: string = {id: "168c9a2987fad481c5882847ac102aaf", defaultMessage: "i am message"}->ReactIntPpxAdaptor.Message.to_s
```
- Ppx supports `intl.el` annotation:
```reason
let element: React.element = [%intl.el "i am message"]
```
Which will be converted to the:

```reason
let element: React.element = {id: "168c9a2987fad481c5882847ac102aaf", defaultMessage: "i am message"}->ReactIntPpxAdaptor.Message.to_s->React.string
```

To use this features you have to define `ReactIntlPpxAdaptor` in your app (see test folder for details).
