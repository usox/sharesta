<?hh // strict
namespace Usox\Sharesta;

final class Request implements RequestInterface {

	private Map<string, string> $uri_values = Map {};

	private Map<string, string> $route_parameters = Map {};

	private string $route = '';

	private string $http_method = '';

	public function __construct(
		string $basedir,
		Map<string,string> $server_variables,
		Map<string,string> $get_variables,
		private RequestBody $request_body
	) {
		// establish the route property
		$this->route = str_replace($basedir, '', $server_variables->get('REQUEST_URI'));
		if ($server_variables->contains('QUERY_STRING')) {
			$this->route = str_replace('?' . $server_variables->get('QUERY_STRING') , '', $this->route);
		}
		$this->route = rtrim($this->route, '/');
		if ($this->route == '') {
			$this->route = '/';
		}
		$this->route = str_replace('//','/', $this->route);

		// establish the http_method property
		$http_method = $server_variables->get('REQUEST_METHOD');
		if ($http_method !== null) {
			$this->http_method = $http_method;
		}

		foreach ($get_variables as $key => $value) {
			$this->uri_values->add(Pair {$key, $value});
		}
	}

	public function getRoute(): string {
		return $this->route;
	}

	public function getHttpMethod(): string {
		return $this->http_method;
	}

	public function setRouteParameters(Map<string,string> $parameters): void {
		$this->route_parameters = $parameters;
	}

	public function getRouteParameters(): Map<string,string> {
		return $this->route_parameters;
	}

	public function getUriValues(): Map<string,string> {
		return $this->uri_values;
	}
	
	public function getBodyAsJson(): string {
		return json_encode($this->request_body->getBody()); 
	}

	public function getBody(): Map<string,string> {
		return $this->request_body->getBody();
	}
}
