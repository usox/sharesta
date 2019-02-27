namespace Usox\Sharesta;

use type Facebook\HackRouter\HttpMethod;
use type Facebook\HackRouter\NotFoundException;

final class Sharesta implements SharestaInterface {

  private vec<Route\RouteInterface> $routes = vec[];

  public function registerRoutes(
    vec<classname<Contract\SharestaApplicationInterface>> $handler_list
  ): this {
    foreach ($handler_list as $handler) {
      $this->routes[] = \call_user_func(
        [$handler, 'registerRoute'],
        new Route\RouteBuilder()
      );
    }

    return $this;
  }

  public async function routeAsync(): Awaitable<void> {
    $router = new Router($this->routes);
    $server_request = \Usox\HackTTP\createServerRequestFromGlobals();

    $code = 200;
    $result = null;

    try {
      list($responder, $params) = $router->routeRequest($server_request);

      $result = $responder($params, $server_request);
      $reason = '';
    } catch (NotFoundException $e) {
      $code = 404;
      $reason = 'Not found';
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

    if ($result !== null) {
      $response->getBody()->rawWriteBlocking(\json_encode($result));
    }

    await (new \Usox\HackTTP\Response\TemporaryFileSapiEmitter())->emitAsync($response);
  }
}
