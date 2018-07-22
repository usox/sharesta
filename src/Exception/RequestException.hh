<?hh // strict
namespace Usox\Sharesta\Exception;

class RequestException extends SharestaException {

	<<__Override>>
	public function getHttpStatusCode(): int {
		return \Usox\Sharesta\Router::HTTP_BAD_REQUEST;
	}
}
