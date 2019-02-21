<?hh // strict
namespace Usox\Sharesta\Example;

require_once '../../vendor/hh_autoload.hh';

use Usox\Sharesta\ApiFactory;
use Usox\Sharesta\RequestInterface;
use Usox\Sharesta\RouterInterface;
use Usox\Sharesta\RoutesInterface;

class HomeRoute implements \JsonSerializable {

	public function jsonSerialize(): string {
		return 'Welcome home';
	}
}

class GetSomeIdRoute implements \JsonSerializable {

	private ?string $request_id;

	public function __construct(private RequestInterface $request): void {
	}

	public function doSomeFancyMagic(): void {
		$this->request_id = $this->request->getRouteParameters()['id'];
	}

	public function jsonSerialize(): string {
		return 'Got '.(string) $this->request_id;
	}
}

class Routes implements RoutesInterface {

	public function registerRoutes(RouterInterface $router): void {
		$router->get('/', (RequestInterface $request): \JsonSerializable ==> {
			return new HomeRoute();
		});
		$router->get('/get_some/:id', (RequestInterface $request): \JsonSerializable ==> {
			$route = new GetSomeIdRoute($request);
			$route->doSomeFancyMagic();
			return $route;
		});
	}
}

<<__EntryPoint>>
function main(): noreturn {
	/* HH_IGNORE_ERROR[2050] */ $get_vars = dict($_GET);
	/* HH_IGNORE_ERROR[2050] */ $server_vars = dict($_SERVER);
	$factory = new ApiFactory();

	$router = $factory->createRouter(
		$server_vars,
		$get_vars
	);

	$routes = new Routes();
	$routes->registerRoutes($router);

	$router->route('/example/webroot/index.hh');

	die(0);
}