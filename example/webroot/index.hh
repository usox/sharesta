<?hh // decl
namespace Usox\Sharesta\Example;

require_once '../../vendor/autoload.php';

use Usox\Sharesta\Exception;
use Usox\Sharesta\RequestInterface;
use Usox\Sharesta\Request;
use Usox\Sharesta\RequestBody;
use Usox\Sharesta\Router;
use Usox\Sharesta\Application;
use Usox\Sharesta\Response;

class HomeRoute implements \JsonSerializable {

	public function jsonSerialize(): string {
		return 'Welcome home';
	}
}

class GetSomeIdRoute implements \JsonSerializable {

	public function __construct(private RequestInterface $request): void {
	}

	public function doSomeFancyMagic(): void {
		$this->request_id = $this->request->getRouteParameters()->get('id');
	}

	public function jsonSerialize(): string {
		return 'Got '.$this->request_id;
	}
}

class Routes implements \Usox\Sharesta\RoutesInterface {

	public function registerRoutes(\Usox\Sharesta\RouterInterface $router): void {
		$router->get('/', function (Request $request) {
			return new HomeRoute();
		});
		$router->get('/get_some/:id', function (Request $request) {
			$route = new GetSomeIdRoute($request);
			$route->doSomeFancyMagic();
			return $route;
		});
	}
}

function init() {

	try {
		$request = new Request(
			'TEST/example/webroot',
			new Map($_SERVER),
			new Map($_GET),
			new RequestBody()
		);
		$app = new Usox\Sharesta\Application(
			$request,
			new Router(),
			new Routes()
		);
		$app->init();
		$app->execute();
	} catch (Exception\NotFoundException $e) {
		$response = new Response();
		$response->send(404, $e->getMessage());
	} catch (Exception\RequestException $e) {
		$response = new Response();
		$response->send(400, $e->getMessage());
	} catch (Exception\ServerException $e) {
		$response = new Response();
		$response->send(500, $e->getMessage());
	}
}
init();
