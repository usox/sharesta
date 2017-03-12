<?hh // partial
namespace Usox\Sharesta\Example;

require_once '../../vendor/autoload.php';

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
		$this->request_id = $this->request->getRouteParameters()->get('id');
	}

	public function jsonSerialize(): string {
		return 'Got '.(string) $this->request_id;
	}
}

class Routes implements RoutesInterface {

	public function registerRoutes(RouterInterface $router): void {
		$router->get('/', function (RequestInterface $request) {
			return new HomeRoute();
		});
		$router->get('/get_some/:id', function (RequestInterface $request) {
			$route = new GetSomeIdRoute($request);
			$route->doSomeFancyMagic();
			return $route;
		});
	}
}

function init() {
	$factory = new ApiFactory();

	$router = $factory->createRouter();
	$request = $factory->createRequest(
		$router,
		new Map($_SERVER),
		new Map($_GET),
	);

	$routes = new Routes();
	$routes->registerRoutes($router);

	$factory->createApplication(
		$request,
		$router
	)
	->handle('/PATH/TO/BASEDIR');
}
init();
