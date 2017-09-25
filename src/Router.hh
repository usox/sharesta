<?hh // strict
namespace Usox\Sharesta;

final class Router implements RouterInterface {

	public function __construct(private RequestInterface $request): void {
	}

	private Map<string, (function (RequestInterface): \JsonSerializable)> $routes = Map {};

	public function register(string $route, (function (RequestInterface): \JsonSerializable) $callback, ?string $http_method = null): void {
		if ($http_method !== null) {
			$route = sprintf(
				'%s:%s',
				$http_method,
				$route
			);
		}

		$this->routes->add(Pair {$route, $callback});
	}

	public function get(string $route, (function (RequestInterface): \JsonSerializable) $callback): void {
		$this->register($route, $callback, 'GET');
	}

	public function post(string $route, (function (RequestInterface): \JsonSerializable) $callback): void {
		$this->register($route, $callback, 'POST');
	}

	public function put(string $route, (function (RequestInterface): \JsonSerializable) $callback): void {
		$this->register($route, $callback, 'PUT');
	}

	public function delete(string $route, (function (RequestInterface): \JsonSerializable) $callback): void {
		$this->register($route, $callback, 'DELETE');
	}

	public function route(string $base_path): \JsonSerializable {
		$requested_route = $this->request->getRoute($base_path);
		$http_method = $this->request->getHttpMethod();
		$route_parameters = Map{};

		foreach ($this->routes as $route => $callback) {
			// if the route also defines a HTTP method, use it as prefix for the lookup
			$route_method = strstr($route, ':', true);
			$request_prepend = '';

			if ($route_method && substr($route_method, -1) != '/') {
				$request_prepend = sprintf('%s:', $http_method);
			}

			$route_pattern = preg_replace(
				'%/:([^ /?]+)(\?)?%',
				'/\2(?P<\1>[^ /?]+)\2',
				$route
			);

			$uri_params = [];
			if (preg_match(
				'%^'.$route_pattern.'$%',
				$request_prepend.$requested_route,
				$uri_params)
			) {
				if ($callback === null) {
					throw new Exception\NotFoundException('The requested resource was not found');
				}

				if (!is_callable($callback)) {
					throw new Exception\ServerException('The provided route callback is not callable');
				}

				foreach ($uri_params as $key => $value) {
					if (!is_numeric($key)) {
						$route_parameters->add(Pair {$key, $value});
					}
				}
				$this->request->setRouteParameters($route_parameters->toImmMap());

				return $callback($this->request);
			}
		}
		throw new Exception\NotFoundException('The requested resource was not found');
	}
}
