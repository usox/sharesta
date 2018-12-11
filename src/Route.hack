namespace Usox\Sharesta;

use type Facebook\HackRouter\HttpMethod;

final class Route {

  public function __construct(
    private HttpMethod $method,
    private string $path,
    private ResponderType $responder
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
