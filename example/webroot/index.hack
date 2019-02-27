namespace Usox\Sharesta\Example;

require_once '../../vendor/hh_autoload.hh';

use type Usox\Sharesta\Sharesta;
use type Usox\Sharesta\Route\RouteBuilderInterface;
use type Usox\Sharesta\Route\RouteInterface;
use type Facebook\HackRouter\HttpMethod;
use type Facebook\HackRouter\UriPattern;
use type Facebook\Experimental\Http\Message\ServerRequestInterface;
use type Usox\Sharesta\Contract\SharestaApplicationInterface;

final class SampleRouteHandler implements SharestaApplicationInterface {

	public function __construct(
		private ImmMap<string, string> $params,
    private ServerRequestInterface $server_request
	) {
	}

	public function jsonSerialize(): mixed {
		return 'Welcome home';
	}

  public static function registerRoute(
    RouteBuilderInterface $route_builder
  ): RouteInterface {
    return $route_builder->setMethod(HttpMethod::GET)
      ->setPath('/')
      ->setResponder(
        ($params, $server_request) ==>
          new SampleRouteHandler($params, $server_request),
      )
      ->build();
  }
}

final class ParamRouteHandler implements SharestaApplicationInterface {

	public function __construct(
		private ImmMap<string, string> $params,
    private ServerRequestInterface $server_request
	) {
	}

	public function jsonSerialize(): mixed {
		return 'Welcome '.$this->params->get('foobar');
	}

  public static function registerRoute(
    RouteBuilderInterface $route_builder
  ): RouteInterface {
    return $route_builder->setMethod(HttpMethod::GET)
      ->setPath('/{foobar}')
      ->setResponder(
        ($params, $server_request) ==>
          new ParamRouteHandler($params, $server_request),
      )
      ->build();
  }
}

<<__EntryPoint>>
async function main(): Awaitable<noreturn> {
	$sharesta = new Sharesta();

  $sharesta->registerRoutes(vec[
    SampleRouteHandler::class,
    ParamRouteHandler::class
  ]);

	await $sharesta->routeAsync();

	die(0);
}
