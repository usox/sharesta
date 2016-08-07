<?hh
namespace Usox\Sharesta;

interface RouterInterface {

	public function register(string $route, (function (RequestInterface): \JsonSerializable) $callback, string $httpMethod = ''): void;

	public function get(string $route, (function (RequestInterface): \JsonSerializable) $callback): void;

	public function post(string $route, (function (RequestInterface): \JsonSerializable) $callback): void;

	public function put(string $route, (function (RequestInterface): \JsonSerializable) $callback): void;

	public function delete(string $route, (function (RequestInterface): \JsonSerializable) $callback): void;

	public function getRouteParameters(): Map<string,string>;

	public function matchRequest(RequestInterface $request): (function (RequestInterface): \JsonSerializable);
}
