<?hh // strict
namespace Usox\Sharesta;

final class Response implements ResponseInterface {

	private Map<int, string> $codes = Map {
		200 => 'OK',
		201 => 'Created',
		202 => 'Accepted',
		204 => 'No Content',
		301 => 'Moved Permanently',
		302 => 'Found',
		304 => 'Not Modified',
		307 => 'Temporary Redirect',
		308 => 'Permanent Redirect',
		400 => 'Bad Request',
		401 => 'Unauthorized',
		403 => 'Forbidden',
		404 => 'Not Found',
		405 => 'Method Not Allowed',
		406 => 'Not Acceptable',
		410 => 'Gone',
		415 => 'Unsupported Media Type',
		417 => 'Expectation Failed',
		429 => 'Too Many Requests',
		500 => 'Internal Server Error',
		501 => 'Not Implemented'
	};

	public function __construct(
		private int $code,
		private mixed $body
	) {}

	public function send(): void {
		if ($this->codes->contains($this->code)) {
			if (!$this->body) {
				$this->body = $this->codes->get($this->code);
			}
		} else {
			$this->code = 500;
			$this->body = 'API attempted to return an unknown HTTP status.';
		}

		header(
			sprintf(
				'HTTP/1.1 %d %s' ,
				$this->code,
				$this->codes->get($this->code)
			)
		);
		header('Content-type: application/json');
		echo json_encode($this->body);
	}
}
