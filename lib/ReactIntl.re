type dateTimeFormatOptions;

[@mel.obj]
external dateTimeFormatOptions:
  (
    ~localeMatcher: [@mel.string] [ | [@mel.as "best fit"] `bestFit | `lookup]=?,
    ~formatMatcher: [@mel.string] [ | [@mel.as "best fit"] `bestFit | `basic]=?,
    ~timeZone: string=?,
    ~hour12: bool=?,
    ~weekday:  [ | `narrow | `short | `long]=?,
    ~era:  [ | `narrow | `short | `long]=?,
    ~year: [@mel.string] [ | `numeric | [@mel.as "2-digit"] `twoDigit]=?,
    ~month: [@mel.string] [
              | `numeric
              | [@mel.as "2-digit"] `twoDigit
              | `narrow
              | `short
              | `long
            ]
              =?,
    ~day: [@mel.string] [ | `numeric | [@mel.as "2-digit"] `twoDigit]=?,
    ~hour: [@mel.string] [ | `numeric | [@mel.as "2-digit"] `twoDigit]=?,
    ~minute: [@mel.string] [ | `numeric | [@mel.as "2-digit"] `twoDigit]=?,
    ~second: [@mel.string] [ | `numeric | [@mel.as "2-digit"] `twoDigit]=?,
    ~timeZoneName: [ | `short | `long]=?,
    ~format: string=?,
    unit
  ) =>
  dateTimeFormatOptions;

type relativeTimeFormatOptions;

[@mel.obj]
external relativeTimeFormatOptions:
  (
    ~numeric:  [ | `always | `auto]=?,
    ~style:  [ | `long | `short | `narrow]=?,
    ~format: string=?,
    unit
  ) =>
  relativeTimeFormatOptions;

type numberFormatOptions;

[@mel.obj]
external numberFormatOptions:
  (
    ~localeMatcher: [@mel.string] [ | [@mel.as "best fit"] `bestFit | `lookup]=?,
    ~style:  [ | `decimal | `currency | `percent]=?,
    ~currency: string=?,
    ~currencyDisplay:  [ | `symbol | `code | `name]=?,
    ~useGrouping: bool=?,
    ~minimumIntegerDigits: int=?,
    ~minimumFractionDigits: int=?,
    ~maximumFractionDigits: int=?,
    ~minimumSignificantDigits: int=?,
    ~maximumSignificantDigits: int=?,
    unit
  ) =>
  numberFormatOptions;

type pluralFormatOptions;

[@mel.obj]
external pluralFormatOptions:
  (~style:  [ | `cardinal | `ordinal]=?, unit) =>
  pluralFormatOptions;

type listFormatOptions;

[@mel.obj]
external listFormatOptions:
  (
    ~style:  [ | `long | `short | `narrow]=?,
    ~_type:  [ | `disjunction | `conjunction | `unit]=?,
    unit
  ) =>
  listFormatOptions;

type displayNameFormatOptions;

[@mel.obj]
external displayNameFormatOptions:
  (
    ~style:  [ | `long | `short | `narrow]=?,
    ~_type:  [ | `language | `region | `script | `currency]=?,
    ~fallback:  [ | `code | `none]=?,
    unit
  ) =>
  displayNameFormatOptions;

type message = {
  id: string,
  defaultMessage: string,
};

type part = {
  [@mel.as "type"]
  type_: string,
  value: string,
};

module Intl = {
  type t;

  [@mel.get] external locale: t => string = "locale";
  [@mel.get] [@mel.return nullable]
  external timeZone: t => option(string) = "timeZone";
  [@mel.get] external formats: t => Js.t({..}) = "formats";
  [@mel.get] external messages: t => Js.Dict.t(string) = "messages";
  [@mel.get] external defaultLocale: t => string = "defaultLocale";
  [@mel.get] external defaultFormats: t => Js.t({..}) = "defaultFormats";
  [@mel.send] external formatDate: (t, Js.Date.t) => string = "formatDate";
  [@mel.send]
  external formatDateToParts: (t, Js.Date.t) => array(part) =
    "formatDateToParts";
  [@mel.send]
  external formatDateWithOptions:
    (t, Js.Date.t, dateTimeFormatOptions) => string =
    "formatDate";
  [@mel.send]
  external formatDateWithOptionsToParts:
    (t, Js.Date.t, dateTimeFormatOptions) => array(part) =
    "formatDateToParts";
  [@mel.send] external formatTime: (t, Js.Date.t) => string = "formatTime";
  [@mel.send]
  external formatTimeToParts: (t, Js.Date.t) => array(part) =
    "formatTimeToParts";
  [@mel.send]
  external formatTimeWithOptions:
    (t, Js.Date.t, dateTimeFormatOptions) => string =
    "formatTime";
  [@mel.send]
  external formatTimeWithOptionsToParts:
    (t, Js.Date.t, dateTimeFormatOptions) => array(part) =
    "formatTimeToParts";
  [@mel.send]
  external formatRelativeTime: (t, float) => string = "formatRelativeTime";
  [@mel.send]
  external formatRelativeTimeWithUnit:
    (
      t,
      float,
       [
        | `second
        | `minute
        | `hour
        | `day
        | `week
        | `month
        | `quarter
        | `year
      ]
    ) =>
    string =
    "formatRelativeTime";
  [@mel.send]
  external formatRelativeTimeWithUnitAndOptions:
    (
      t,
      float,
       [
        | `second
        | `minute
        | `hour
        | `day
        | `week
        | `month
        | `quarter
        | `year
      ],
      relativeTimeFormatOptions
    ) =>
    string =
    "formatRelativeTime";
  [@mel.send] external formatNumber: (t, float) => string = "formatNumber";
  [@mel.send]
  external formatNumberToParts: (t, float) => array(part) =
    "formatNumberToParts";
  [@mel.send]
  external formatNumberWithOptions: (t, float, numberFormatOptions) => string =
    "formatNumber";
  [@mel.send]
  external formatNumberWithOptionsToParts:
    (t, float, numberFormatOptions) => array(part) =
    "formatNumberToParts";
  [@mel.send] external formatPlural: (t, int) => string = "formatPlural";
  [@mel.send]
  external formatPluralWithOptions: (t, int, pluralFormatOptions) => string =
    "formatPlural";
  [@mel.send] external formatMessage: (t, message) => string = "formatMessage";
  [@mel.send]
  external formatMessageWithValues: (t, message, Js.t({..})) => string =
    "formatMessage";
  [@mel.send] external formatList: (t, array(string)) => string = "formatList";
  [@mel.send]
  external formatListWithOptions:
    (t, array(string), listFormatOptions) => string =
    "formatList";
  [@mel.send]
  external formatDisplayName: (t, string) => string = "formatDisplayName";
  [@mel.send]
  external formatDisplayNameWithOptions:
    (t, string, displayNameFormatOptions) => string =
    "formatDisplayName";
};

[@mel.module "react-intl"]
external context: React.Context.t(Intl.t) = "IntlContext";

[@mel.module "react-intl"] external useIntl: unit => Intl.t = "useIntl";

type textComponent;

external domTag: string => textComponent = "%identity";
external textComponent: React.component('props) => textComponent =
  "%identity";

type intlCache;

[@mel.module "react-intl"]
external createIntlCache: unit => intlCache = "createIntlCache";

type intlConfig;

[@mel.obj]
external intlConfig:
  (
    ~locale: string,
    ~timeZone: string=?,
    ~formats: Js.t({..})=?, /* TODO */
    ~textComponent: textComponent=?,
    ~messages: Js.Dict.t(string),
    ~defaultLocale: string=?,
    ~defaultFormats: Js.t({..})=?, /* TODO */
    ~onError: Js.Exn.t => unit=?,
    unit
  ) =>
  intlConfig;

[@mel.module "react-intl"]
external createIntl: (intlConfig, intlCache) => Intl.t = "createIntl";

module RawIntlProvider = {
  [@react.component] [@mel.module "react-intl"]
  external ake: (~value: Intl.t, ~children: React.element) => React.element =
    "RawIntlProvider";
};

module IntlProvider = {
  [@react.component] [@mel.module "react-intl"]
  external make:
    (
      ~locale: string=?,
      ~formats: Js.t({..})=?, /* TODO */
      ~messages: Js.Dict.t(string)=?,
      ~defaultLocale: string=?,
      ~defaultFormats: Js.t({..})=?, /* TODO */
      ~textComponent: textComponent=?,
      ~initialNow: int=?,
      ~onError: string => unit=?,
      ~children: React.element
    ) =>
    React.element =
    "IntlProvider";
};

module FormattedMessage = {
  [@react.component] [@mel.module "react-intl"]
  external make:
    (
      ~id: string,
      ~defaultMessage: string,
      ~values: Js.t({..})=?,
      ~tagName: string=?,
      ~children: (~formattedMessage: React.element) => React.element=?
    ) =>
    React.element =
    "FormattedMessage";
};

module FormattedDate = {
  [@react.component] [@mel.module "react-intl"]
  external make:
    (
      ~value: Js.Date.t,
      ~localeMatcher: [@mel.string] [ | [@mel.as "best fit"] `bestFit | `lookup]
                        =?,
      ~formatMatcher: [@mel.string] [ | [@mel.as "best fit"] `bestFit | `basic]=?,
      ~timeZone: string=?,
      ~hour12: bool=?,
      ~weekday:  [ | `narrow | `short | `long]=?,
      ~era:  [ | `narrow | `short | `long]=?,
      ~year: [@mel.string] [ | `numeric | [@mel.as "2-digit"] `twoDigit]=?,
      ~month: [@mel.string] [
                | `numeric
                | [@mel.as "2-digit"] `twoDigit
                | `narrow
                | `short
                | `long
              ]
                =?,
      ~day: [@mel.string] [ | `numeric | [@mel.as "2-digit"] `twoDigit]=?,
      ~hour: [@mel.string] [ | `numeric | [@mel.as "2-digit"] `twoDigit]=?,
      ~minute: [@mel.string] [ | `numeric | [@mel.as "2-digit"] `twoDigit]=?,
      ~second: [@mel.string] [ | `numeric | [@mel.as "2-digit"] `twoDigit]=?,
      ~timeZoneName:  [ | `short | `long]=?,
      ~format: string=?,
      ~children: (~formattedDate: string) => React.element=?
    ) =>
    React.element =
    "FormattedDate";
};

module FormattedDateParts = {
  [@react.component] [@mel.module "react-intl"]
  external make:
    (
      ~value: Js.Date.t,
      ~localeMatcher: [@mel.string] [ | [@mel.as "best fit"] `bestFit | `lookup]
                        =?,
      ~formatMatcher: [@mel.string] [ | [@mel.as "best fit"] `bestFit | `basic]=?,
      ~timeZone: string=?,
      ~hour12: bool=?,
      ~weekday:  [ | `narrow | `short | `long]=?,
      ~era:  [ | `narrow | `short | `long]=?,
      ~year: [@mel.string] [ | `numeric | [@mel.as "2-digit"] `twoDigit]=?,
      ~month: [@mel.string] [
                | `numeric
                | [@mel.as "2-digit"] `twoDigit
                | `narrow
                | `short
                | `long
              ]
                =?,
      ~day: [@mel.string] [ | `numeric | [@mel.as "2-digit"] `twoDigit]=?,
      ~hour: [@mel.string] [ | `numeric | [@mel.as "2-digit"] `twoDigit]=?,
      ~minute: [@mel.string] [ | `numeric | [@mel.as "2-digit"] `twoDigit]=?,
      ~second: [@mel.string] [ | `numeric | [@mel.as "2-digit"] `twoDigit]=?,
      ~timeZoneName:  [ | `short | `long]=?,
      ~format: string=?,
      ~children: (~formattedDateParts: array(part)) => React.element
    ) =>
    React.element =
    "FormattedDateParts";
};

module FormattedTime = {
  [@react.component] [@mel.module "react-intl"]
  external make:
    (
      ~value: Js.Date.t,
      ~localeMatcher: [@mel.string] [ | [@mel.as "best fit"] `bestFit | `lookup]
                        =?,
      ~formatMatcher: [@mel.string] [ | [@mel.as "best fit"] `bestFit | `basic]=?,
      ~timeZone: string=?,
      ~hour12: bool=?,
      ~weekday:  [ | `narrow | `short | `long]=?,
      ~era:  [ | `narrow | `short | `long]=?,
      ~year: [@mel.string] [ | `numeric | [@mel.as "2-digit"] `twoDigit]=?,
      ~month: [@mel.string] [
                | `numeric
                | [@mel.as "2-digit"] `twoDigit
                | `narrow
                | `short
                | `long
              ]
                =?,
      ~day: [@mel.string] [ | `numeric | [@mel.as "2-digit"] `twoDigit]=?,
      ~hour: [@mel.string] [ | `numeric | [@mel.as "2-digit"] `twoDigit]=?,
      ~minute: [@mel.string] [ | `numeric | [@mel.as "2-digit"] `twoDigit]=?,
      ~second: [@mel.string] [ | `numeric | [@mel.as "2-digit"] `twoDigit]=?,
      ~timeZoneName:  [ | `short | `long]=?,
      ~format: string=?,
      ~children: (~formattedTime: string) => React.element=?
    ) =>
    React.element =
    "FormattedTime";
};

module FormattedTimeParts = {
  [@react.component] [@mel.module "react-intl"]
  external make:
    (
      ~value: Js.Date.t,
      ~localeMatcher: [@mel.string] [ | [@mel.as "best fit"] `bestFit | `lookup]
                        =?,
      ~formatMatcher: [@mel.string] [ | [@mel.as "best fit"] `bestFit | `basic]=?,
      ~timeZone: string=?,
      ~hour12: bool=?,
      ~weekday:  [ | `narrow | `short | `long]=?,
      ~era:  [ | `narrow | `short | `long]=?,
      ~year: [@mel.string] [ | `numeric | [@mel.as "2-digit"] `twoDigit]=?,
      ~month: [@mel.string] [
                | `numeric
                | [@mel.as "2-digit"] `twoDigit
                | `narrow
                | `short
                | `long
              ]
                =?,
      ~day: [@mel.string] [ | `numeric | [@mel.as "2-digit"] `twoDigit]=?,
      ~hour: [@mel.string] [ | `numeric | [@mel.as "2-digit"] `twoDigit]=?,
      ~minute: [@mel.string] [ | `numeric | [@mel.as "2-digit"] `twoDigit]=?,
      ~second: [@mel.string] [ | `numeric | [@mel.as "2-digit"] `twoDigit]=?,
      ~timeZoneName:  [ | `short | `long]=?,
      ~format: string=?,
      ~children: (~formattedTimeParts: array(part)) => React.element
    ) =>
    React.element =
    "FormattedTimeParts";
};

module FormattedRelativeTime = {
  [@react.component] [@mel.module "react-intl"]
  external make:
    (
      ~value: float,
      ~unit:  [
               | `second
               | `minute
               | `hour
               | `day
               | `week
               | `month
               | `quarter
               | `year
             ]
               =?,
      ~numeric:  [ | `always | `auto]=?,
      ~style:  [ | `long | `short | `narrow]=?,
      ~format: string=?,
      ~updateIntervalInSeconds: float=?,
      ~children: (~formattedDate: string) => React.element=?
    ) =>
    React.element =
    "FormattedRelativeTime";
};

module FormattedNumber = {
  [@react.component] [@mel.module "react-intl"]
  external make:
    (
      ~value: float,
      ~localeMatcher: [@mel.string] [ | [@mel.as "best fit"] `bestFit | `lookup]
                        =?,
      ~style:  [ | `decimal | `currency | `percent]=?,
      ~currency: string=?,
      ~currencyDisplay:  [ | `symbol | `code | `name]=?,
      ~useGrouping: bool=?,
      ~minimumIntegerDigits: int=?,
      ~minimumFractionDigits: int=?,
      ~maximumFractionDigits: int=?,
      ~minimumSignificantDigits: int=?,
      ~maximumSignificantDigits: int=?,
      ~format: string=?,
      ~children: (~formattedNumber: string) => React.element=?
    ) =>
    React.element =
    "FormattedNumber";
};

module FormattedNumberParts = {
  [@react.component] [@mel.module "react-intl"]
  external make:
    (
      ~value: float,
      ~localeMatcher: [@mel.string] [ | [@mel.as "best fit"] `bestFit | `lookup]
                        =?,
      ~style:  [ | `decimal | `currency | `percent]=?,
      ~currency: string=?,
      ~currencyDisplay:  [ | `symbol | `code | `name]=?,
      ~useGrouping: bool=?,
      ~minimumIntegerDigits: int=?,
      ~minimumFractionDigits: int=?,
      ~maximumFractionDigits: int=?,
      ~minimumSignificantDigits: int=?,
      ~maximumSignificantDigits: int=?,
      ~format: string=?,
      ~children: (~formattedNumberParts: array(part)) => React.element
    ) =>
    React.element =
    "FormattedNumberParts";
};

module FormattedPlural = {
  [@react.component] [@mel.module "react-intl"]
  external make:
    (
      ~value: int,
      ~style:  [ | `cardinal | `ordinal]=?,
      ~other: React.element,
      ~zero: React.element=?,
      ~one: React.element=?,
      ~two: React.element=?,
      ~few: React.element=?,
      ~many: React.element=?,
      ~children: (~formattedPlural: string) => React.element=?
    ) =>
    React.element =
    "FormattedPlural";
};

module FormattedList = {
  [@react.component] [@mel.module "react-intl"]
  external make:
    (
      ~value: array(string),
      ~style:  [ | `long | `short | `narrow]=?,
      ~_type:  [ | `disjunction | `conjunction | `unit]=?,
      ~children: (~formattedList: string) => React.element=?
    ) =>
    React.element =
    "FormattedList";
};

module FormattedDisplayName = {
  [@react.component] [@mel.module "react-intl"]
  external make:
    (
      ~value: string,
      ~style:  [ | `long | `short | `narrow]=?,
      ~_type:  [ | `language | `region | `script | `currency]=?,
      ~fallback:  [ | `code | `none]=?
    ) =>
    React.element =
    "FormattedDisplayName";
};
