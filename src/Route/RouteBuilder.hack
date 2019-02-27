namespace Usox\Sharesta\Route;

use type Facebook\HackRouter\HttpMethod;
use type Usox\Sharesta\ResponderType;

final class RouteBuilder implements RouteBuilderInterface {

  private ?HttpMethod $method;

  private ?string $path;

  private ?ResponderType $responder;

  private bool $debug = false;

  public function setMethod(HttpMethod $method): this {
    $this->method = $method;

    return $this;
  }

  public function setPath(string $path): this {
    $this->path = $path;

    return $this;
  }

  public function setResponder(ResponderType $responder): this {
    $this->responder = $responder;

    return $this;
  }

  public function build(): RouteInterface {
    if (
      $this->path !== null &&
      $this->method !== null &&
      $this->responder !== null
    ) {
      return new Route(
        $this->method,
        $this->path,
        $this->responder,
      );
    }
    throw new \InvalidArgumentException();
  }
}
