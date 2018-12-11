namespace Usox\Sharesta;

use type Facebook\HackRouter\HttpMethod;

final class Sharesta {

  private vec<Route> $routes = vec[];

  public function registerRoute(
    HttpMethod $method,
    string $path,
    ResponderType $responder
  ): void {
    $this->routes[] = new Route(
      $method,
      $path,
      $responder
    );
  }

  public async function routeAsync(): Awaitable<void> {
    $router = new Router($this->routes);
    $server_request = \Usox\HackTTP\createServerRequestFromGlobals();

    list($responder, $params) = $router->routeRequest($server_request);

    $code = 200;
    $result = '';

    try {
      $result = $responder($params);
      $reason = '';
    } catch (Exception\SharestaException $e) {
      $code = $e->getHttpStatusCode();
      $reason = $e->getMessage();
    } catch (\Exception $exception) {
      $code = 500;
      $reason = 'Internal server error';
    }

    $response = \Usox\HackTTP\createResponse(
      $code,
      $reason
    )->withAddedHeaderLine('Content-type', 'application/json');

    $response->getBody()->rawWriteBlocking(\json_encode($result));

    await (new \Usox\HackTTP\Response\TemporaryFileSapiEmitter())->emitAsync($response);
  }
}
