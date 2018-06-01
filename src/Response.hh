<?hh // strict
namespace Usox\Sharesta;

use HH\Lib\Str;

final class Response implements ResponseInterface {

	private Map<int, string> $codes = Map {
		Router::HTTP_OK => 'OK',
		Router::HTTP_BAD_REQUEST => 'Bad Request',
		Router::HTTP_NOT_FOUND => 'Not Found',
		Router::HTTP_INTERNAL_SERVER_ERROR => 'Internal Server Error',
	};

	public function __construct(
		private int $code,
		private ?string $body
	) {}

	public function send(): void {
		if ($this->codes->contains($this->code)) {
			if (null === $this->body) {
				$this->body = \json_encode(
					$this->codes->get($this->code)
				);
			}
		} else {
			$this->code = 500;
			$this->body = \json_encode('API attempted to return an unknown HTTP status.');
		}

		@\header(
			Str\format(
				'HTTP/1.1 %d %s' ,
				$this->code,
				(string) $this->codes->get($this->code)
			)
		);
		@\header('Content-type: application/json');
		echo $this->body;
	}
}
