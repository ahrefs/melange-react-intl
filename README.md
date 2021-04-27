# Examples

```reason
let message: ReactIntl.message = [%intl "blabla"]
let descriptedMessage: ReactIntl.message = [%intl {msg: "blabla", desc: "i am description"}]

let message: string = [%intl.s "blabla"] // type string
let descriptedmessage: string = [%intl.s {msg: "blabla", desc: "i am description"}] // type string

let element: Reason.element = [%intl.el "blabla"]
let descriptedElement: Reason.element = [%intl.el {msg: "blabla", desc: "i am description"}]
```
