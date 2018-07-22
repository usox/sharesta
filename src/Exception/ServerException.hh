<?hh // strict
namespace Usox\Sharesta\Exception;

final class ServerException extends \Exception {

	public function getHttpStatusCode(): int {
		return \Usox\Sharesta\Router::HTTP_INTERNAL_SERVER_ERROR;
	}
}
