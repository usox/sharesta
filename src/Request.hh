<?hh // strict
namespace Usox\Sharesta;

final class Request implements RequestInterface {

	public function __construct(
		private RouterInterface $router,
		private Map<string,string> $server_variables,
		private Map<string,string> $get_variables,
		private RequestBody $request_body
	) {
	}

	<<__Memoize>>
	public function getRoute(string $base_path): string {
		$route = str_replace($base_path, '', $this->server_variables->get('REQUEST_URI'));
		if ($this->server_variables->contains('QUERY_STRING')) {
			$route = str_replace('?' . $this->server_variables->get('QUERY_STRING') , '', $route);
		}
		$route = rtrim($route, '/');
		if ($route == '') {
			$route = '/';
		}
		return str_replace('//','/', $route);
	}

	<<__Memoize>>
	public function getHttpMethod(): string {
		$http_method = $this->server_variables->get('REQUEST_METHOD');
		if ($http_method !== null) {
			return $http_method;
		}
		return '';
	}

	public function getRouteParameters(): Map<string,string> {
		return $this->router->getRouteParameters();
	}

	<<__Memoize>>
	public function getUriValues(): Map<string,string> {
		$uri_values = Map{};

		foreach ($this->get_variables as $key => $value) {
			$uri_values->add(Pair {$key, $value});
		}

		return $uri_values;
	}

	public function getBodyAsJson(): string {
		return json_encode($this->request_body->getBody());
	}

	public function getBody(): Map<string,mixed> {
		return $this->request_body->getBody();
	}
}
