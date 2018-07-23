<?hh // strict
namespace Usox\Sharesta;

use namespace HH\Lib\{C, Str, Vec};

final class Response implements ResponseInterface {

	private dict<int, string> $codes = dict[
		Router::HTTP_OK => 'OK',
		Router::HTTP_BAD_REQUEST => 'Bad Request',
		Router::HTTP_NOT_FOUND => 'Not Found',
		Router::HTTP_INTERNAL_SERVER_ERROR => 'Internal Server Error',
	];

	public function __construct(
		private int $code,
		private ?string $body
	) {}

	public function send(vec<string> $response_header): void {
		if (C\contains_key($this->codes, $this->code)) {
			if (null === $this->body) {
				$this->body = \json_encode(
					$this->codes[$this->code]
				);
			}
		} else {
			$this->code = 500;
			$this->body = \json_encode('API attempted to return an unknown HTTP status.');
		}

		$response_header = Vec\concat(
			vec[Str\format('HTTP/1.1 %d %s', $this->code, (string) $this->codes[$this->code])],
			vec['Content-type: application/json'],
			$response_header,
		);

		foreach ($response_header as $header) {
			@\header($header);
		}

		echo $this->body;
	}
}
