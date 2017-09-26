<?hh // strict
namespace Usox\Sharesta\Exception;

final class NotFoundException extends SharestaException {

	public function getHttpStatusCode(): int {
		return \Usox\Sharesta\Router::HTTP_NOT_FOUND;
	}
}
