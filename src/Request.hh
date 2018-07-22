<?hh // strict
namespace Usox\Sharesta;

use namespace HH\Lib\{C, Str};

final class Request implements RequestInterface {

	private dict<string, string> $route_parameters = dict[];

	public function __construct(
		private dict<string,string> $server_variables,
		private dict<string,string> $get_variables,
		private RequestBody $request_body
	) {
	}

	<<__Memoize>>
	public function getRoute(string $base_path): string {
		$route = Str\replace(
			(string) $this->server_variables['REQUEST_URI'],
			$base_path,
			''
		);

		if (C\contains_key($this->server_variables, 'QUERY_STRING')) {
			$route = Str\format(
				'?%s',
				$this->server_variables['QUERY_STRING']
			) |> Str\replace($route, $$, '');
		}

		$route = \rtrim($route, '/');
		if ($route === '') {
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
		return $this->server_variables['REQUEST_METHOD'] ?? '';
	}

	public function setRouteParameters(
		dict<string, string> $route_parameters
	): void {
		$this->route_parameters = $route_parameters;
	}

	public function getRouteParameters(): dict<string, string> {
		return $this->route_parameters;
	}

	public function getUriValues(): dict<string, string> {
		return $this->get_variables;
	}

	<<__Memoize>>
	public function getBodyAsJson(): string {
		return \json_encode($this->request_body->getBody());
	}

	public function getBody(): dict<string, mixed> {
		return $this->request_body->getBody();
	}
}
