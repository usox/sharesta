namespace Usox\Sharesta\Exception;

abstract class SharestaException extends \Exception {

  abstract public function getHttpStatusCode(): int;
}
