namespace Usox\Sharesta\Route;

use type Facebook\HackRouter\HttpMethod;
use type Usox\Sharesta\ResponderType;

interface RouteBuilderInterface {

  public function setMethod(HttpMethod $method): this;

  public function setPath(string $path): this;

  public function setResponder(ResponderType $responder): this;

  public function build(): RouteInterface;
}
