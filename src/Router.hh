<?hh // strict
namespace Usox\Sharesta;

final class Router implements RouterInterface {

	private Map<string, (function (RequestInterface): \JsonSerializable)> $routes = Map {};

	private Map<string, string> $route_parameters = Map {};

	public function register(string $route, (function (RequestInterface): \JsonSerializable) $callback, string $http_method = ''): void {
		if ($http_method) {
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

	public function getRouteParameters(): Map<string,string> {
		return $this->route_parameters;
	}

	private function matchRequest(RequestInterface $request, string $base_path): (function (RequestInterface): \JsonSerializable) {
		$requested_route = $request->getRoute($base_path);
		$http_method = $request->getHttpMethod();

		foreach ($this->routes as $route => $callback) {

			$route_pattern = preg_replace(
				'%/:([^ /?]+)(\?)?%',
				'/\2(?P<\1>[^ /?]+)\2',
				$route
			);

			// if the route also defined a HTTP method to match against, 
			// append the requested route with the request method
			$route_method = strstr($route, ':', true);
			if ($route_method && substr($route_method, -1) != '/') {
				$requestPrepend = $http_method.':';
			} else {
				$requestPrepend = '';
			}

			$uri_params = [];
			if (preg_match(
				'%^'.$route_pattern.'$%',
				$requestPrepend.$requested_route,
				$uri_params)
			) {
				foreach ($uri_params as $key => $value) {
					if (!is_numeric($key)) {
						$this->route_parameters->add(Pair {$key, $value});
					}
				}
				return $callback;
			}
		}
		throw new Exception\ServerException('The requested resource was not found');
	}

	public function route(RequestInterface $request, string $base_path): \JsonSerializable {
		$callable = $this->matchRequest($request, $base_path);
		if ($callable === null) {
			throw new Exception\NotFoundException('The requested resource was not found');
		}

		if (!is_callable($callable)) {
			throw new Exception\ServerException('The provided route callback is not callable');
		}
		return $callable($request);
	}
}
