<?hh // strict
namespace Usox\Sharesta;

use HH\Lib\Str;

final class Request implements RequestInterface {

	public ImmMap<string, string> $route_parameters = ImmMap{};

	public function __construct(
		private ImmMap<string,string> $server_variables,
		private ImmMap<string,string> $get_variables,
		private RequestBody $request_body
	) {
	}

	<<__Memoize>>
	public function getRoute(string $base_path): string {
		$route = Str\replace(
			(string) $this->server_variables->get('REQUEST_URI'),
			$base_path,
			''
		);

		if ($this->server_variables->contains('QUERY_STRING')) {
			$route = Str\replace(
				$route,
				'?' . $this->server_variables->get('QUERY_STRING')
				,
				'',
			);
		}

		$route = \rtrim($route, '/');
		if ($route == '') {
			$route = '/';
		}

		return Str\replace(
			$route,
			'//',
			'/',
		);
	}

	<<__Memoize>>
	public function getHttpMethod(): string {
		$http_method = $this->server_variables->get('REQUEST_METHOD');
		if ($http_method !== null) {
			return $http_method;
		}
		return '';
	}

	public function setRouteParameters(ImmMap<string, string> $route_parameters): void {
		$this->route_parameters = $route_parameters;
	}

	public function getRouteParameters(): ImmMap<string,string> {
		return $this->route_parameters;
	}

	public function getUriValues(): ImmMap<string,string> {
		return $this->get_variables;
	}

	<<__Memoize>>
	public function getBodyAsJson(): string {
		return \json_encode($this->request_body->getBody());
	}

	public function getBody(): Map<string,mixed> {
		return $this->request_body->getBody();
	}
}
