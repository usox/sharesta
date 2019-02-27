namespace Usox\Sharesta\Route;

use type Facebook\HackRouter\HttpMethod;
use type Usox\Sharesta\ResponderType;

final class Route implements RouteInterface {

  public function __construct(
    private HttpMethod $method,
    private string $path,
    private ResponderType $responder,
  ) {
  }

  public function getMethod(): HttpMethod {
    return $this->method;
  }

  public function getPath(): string {
    return $this->path;
  }

  public function getResponder(): ResponderType {
    return $this->responder;
  }
}
