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

	public function send(int $code, mixed $body = ''): void {
		if ($this->codes->contains($code)) {
			if (!$body) {
				$body = $this->codes->get($code);
			}
		} else {
			$code = 500;
			$body = 'API attempted to return an unknown HTTP status.';
		}

		header(
			sprintf(
				'HTTP/1.1 %d %s' ,
				$code,
				$this->codes->get($code)
			)
		);
		header('Content-type: application/json');
		echo json_encode($body);
	}
}
