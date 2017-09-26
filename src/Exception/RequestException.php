<?hh // strict
namespace Usox\Sharesta\Exception;

final class RequestException extends SharestaException {

	public function getHttpStatusCode(): int {
		return \Usox\Sharesta\Router::HTTP_BAD_REQUEST;
	}
}
