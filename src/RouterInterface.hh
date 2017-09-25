<?hh // strict
namespace Usox\Sharesta;

interface RouterInterface {

	public function register(string $route, (function (RequestInterface): \JsonSerializable) $callback, ?string $http_method = null): void;

	public function get(string $route, (function (RequestInterface): \JsonSerializable) $callback): void;

	public function post(string $route, (function (RequestInterface): \JsonSerializable) $callback): void;

	public function put(string $route, (function (RequestInterface): \JsonSerializable) $callback): void;

	public function delete(string $route, (function (RequestInterface): \JsonSerializable) $callback): void;

	public function route(string $base_path): \JsonSerializable;
}
