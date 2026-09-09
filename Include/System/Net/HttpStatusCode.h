/// \file HttpStatusCode.h
/// \brief Defines HTTP status codes defined for HTTP/1.1 and HTTP/2.

#pragma once

namespace DotNetDupe {
    namespace System {
        namespace Net {
            /// \enum HttpStatusCode
            /// \brief Contains the values of status codes defined for HTTP in RFC 9110 and RFC 7231.
            ///
            /// Standards Conformance: RFC 9110 (HTTP Semantics), RFC 7231, RFC 6585, RFC 7540 (HTTP/2).
            enum class HttpStatusCode {
                /// \brief Equivalent to HTTP status 100. Indicates that the client can continue with its request.
                Continue = 100,
                /// \brief Equivalent to HTTP status 101. Indicates that the protocol version is being changed.
                SwitchingProtocols = 101,
                /// \brief Equivalent to HTTP status 200. Indicates that the request succeeded.
                OK = 200,
                /// \brief Equivalent to HTTP status 201. Indicates that the request resulted in a new resource created.
                Created = 201,
                /// \brief Equivalent to HTTP status 202. Indicates that the request has been accepted for further processing.
                Accepted = 202,
                NonAuthoritativeInformation = 203,
                NoContent = 204,
                ResetContent = 205,
                PartialContent = 206,
                MultipleChoices = 300,
                Ambiguous = 300,
                MovedPermanently = 301,
                Moved = 301,
                Found = 302,
                Redirect = 302,
                SeeOther = 303,
                RedirectMethod = 303,
                NotModified = 304,
                UseProxy = 305,
                Unused = 306,
                TemporaryRedirect = 307,
                RedirectKeepVerb = 307,
                PermanentRedirect = 308,
                BadRequest = 400,
                Unauthorized = 401,
                PaymentRequired = 402,
                Forbidden = 403,
                NotFound = 404,
                MethodNotAllowed = 405,
                NotAcceptable = 406,
                ProxyAuthenticationRequired = 407,
                RequestTimeout = 408,
                Conflict = 409,
                Gone = 410,
                LengthRequired = 411,
                PreconditionFailed = 412,
                RequestEntityTooLarge = 413,
                RequestUriTooLong = 414,
                UnsupportedMediaType = 415,
                RequestedRangeNotSatisfiable = 416,
                ExpectationFailed = 417,
                MisdirectedRequest = 421,
                UnprocessableEntity = 422,
                Locked = 423,
                FailedDependency = 424,
                UpgradeRequired = 426,
                PreconditionRequired = 428,
                TooManyRequests = 429,
                RequestHeaderFieldsTooLarge = 431,
                UnavailableForLegalReasons = 451,
                InternalServerError = 500,
                NotImplemented = 501,
                BadGateway = 502,
                ServiceUnavailable = 503,
                GatewayTimeout = 504,
                HttpVersionNotSupported = 505,
                VariantAlsoNegotiates = 506,
                InsufficientStorage = 507,
                LoopDetected = 508,
                NotExtended = 510,
                NetworkAuthenticationRequired = 511
            };
        }
    }
}
