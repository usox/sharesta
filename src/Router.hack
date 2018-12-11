namespace Usox\Sharesta;

use type Facebook\HackRouter\BaseRouter;
use type Facebook\HackRouter\HttpMethod;
use namespace HH\Lib\{C};

final class Router extends BaseRouter<ResponderType> {

  public function __construct(
    private vec<Route> $routes
  ) {}

  <<__Override>>
  protected function getRoutes(
  ): ImmMap<HttpMethod, ImmMap<string, ResponderType>> {
    $routes = dict[];
    foreach ($this->routes as $route) {
      $method = $route->getMethod();
      if (!C\contains_key($routes, $method)) {
        $routes[$method] = dict[];
      }

      $routes[$method][$route->getPath()] = $route->getResponder();
    }

    $result = dict[];

    foreach ($routes as $method => $route_list) {
      $result[$method] = new ImmMap($route_list);
    }

    return new ImmMap($result);
  }
}
