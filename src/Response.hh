<?hh // strict
namespace Usox\Sharesta;

final class Response implements ResponseInterface {

	private Map<int, string> $codes = Map {
		Application::HTTP_OK => 'OK',
		Application::HTTP_BAD_REQUEST => 'Bad Request',
		Application::HTTP_NOT_FOUND => 'Not Found',
		Application::HTTP_INTERNAL_SERVER_ERROR => 'Internal Server Error',
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
