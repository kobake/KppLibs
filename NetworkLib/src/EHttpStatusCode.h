#pragma once

//http://ja.wikipedia.org/wiki/HTTP%E3%82%B9%E3%83%86%E3%83%BC%E3%82%BF%E3%82%B9%E3%82%B3%E3%83%BC%E3%83%89

enum EHttpStatusCode{
	//1xx Informational
	HTTP_CONTINUE							= 100,	//継続
	HTTP_SWITCHING_PROTOCOLS				= 101,	//プロトコル切り替え
	HTTP_PROCESSING							= 102,	//処理中

	//2xx Success
	HTTP_OK									= 200,	//OK
	HTTP_CREATED							= 201,	//作成
	HTTP_ACCEPTED							= 202,	//受理
	HTTP_NON_AUTHORITATIVE_INFORMATION		= 203,	//信頼できない情報
	HTTP_NO_CONTENT							= 204,	//内容無し
	HTTP_RESET_CONTENT						= 205,	//内容のリセット
	HTTP_PARTIAL_CONTENT					= 206,	//部分的内容
	HTTP_MULTI_STATUS						= 207,	//複数のステータス
	HTTP_IM_USED							= 226,	//IM使用

	//3xx Redirection
	HTTP_MULTIPLE_CHOICES					= 300,
	HTTP_MOVED_PERMANENTLY					= 301,
	HTTP_FOUND								= 302,
	HTTP_SEE_OTHER							= 303,
	HTTP_NOT_MODIFIED						= 304,
	HTTP_USE_PROXY							= 305,
	_HTTP_UNUSED_							= 306,
	HTTP_TEMPORARY_REDIRECT					= 307,

	//4xx Client Error
	HTTP_BAD_REQUEST						= 400,
	HTTP_UNAUTHORIZED						= 401,
	HTTP_PAYMENT_REQUIRED					= 402,
	HTTP_FORBIDDEN							= 403,
	HTTP_NOT_FOUND							= 404,
	HTTP_METHOD_NOT_ALLOWED					= 405,
	HTTP_NOT_ACCEPTABLE						= 406,
	HTTP_PROXY_AUTHENTICATION_REQUIRED		= 407,
	HTTP_REQUEST_TIMEOUT					= 408,
	HTTP_CONFLICT							= 409,
	HTTP_GONE								= 410,
	HTTP_LENGTH_REQUIRED					= 411,
	HTTP_PRECONDITION_FAILED				= 412,
	HTTP_REQUEST_ENTITY_TOO_LARGE			= 413,
	HTTP_REQUEST_URI_TOO_LONG				= 414,
	HTTP_UNSUPPORTED_MEDIA_TYPE				= 415,
	HTTP_REQUESTED_RANGE_NOT_SATISFIABLE	= 416,
	HTTP_EXPECTATION_FAILED					= 417,
	HTTP_I_AM_A_TEAPOT						= 418,
	HTTP_UNPROCESSABLE_ENTITY				= 422,
	HTTP_LOCKED								= 423,
	HTTP_FAILED_DEPENDENCY					= 424,
	HTTP_UPGRADE_REQUIRED					= 426,

	//5xx Server Error
	HTTP_INTERNAL_SERVER_ERROR				= 500,	//サーバ内部エラー
	HTTP_NOT_IMPLEMENTED					= 501,
	HTTP_BAD_GATEWAY						= 502,	//不正なゲートウェイ。ゲートウェイ・プロキシサーバは不正な要求を受け取り、これを拒否した。
	HTTP_SERVICE_UNAVAILABLE				= 503,
	HTTP_GATEWAY_TIMEOUT					= 504,
	HTTP_VERSION_NOT_SUPPORTED				= 505,
	HTTP_VARIANT_ALSO_NEGOTIATES			= 506,
	HTTP_INSUFFICIENT_STORAGE				= 507,
	HTTP_NOT_EXTENDED						= 510,

};
//*/
